#include "Engine.h"
#include "Timer.h"
#include "Action.h"

#include <iostream>

Engine::Engine(int numberOfPlayers) :
        numberOfPlayers(numberOfPlayers) {

    this->settings.setPodsPerPlayer(numberOfPlayers);
    this->settings.setWidth(800);
    this->settings.setHeight(600);

    this->init();
    this->readPlayers();
    this->messaging.write(this->settings.toMessage());
    this->run();
}

void Engine::init() {
    this->settings.addWall(Shared::Wall(300.0f, 200.0f, 48.0f));
    this->settings.addWall(Shared::Wall(200.0f, 300.0f, 32.0f));
    this->settings.addWall(Shared::Wall(340.0f, 100.0f, 40.0f));
    this->settings.addWall(Shared::Wall(500.0f, 500.0f, 30.0f));

    this->settings.addCheckpoint(Shared::Checkpoint(500.0f, 150.0f, 48.0f));
    this->settings.addCheckpoint(Shared::Checkpoint(650.0f, 400.0f, 36.0f));
}

void Engine::readPlayers() {
    this->messaging.read("players", [&](const Shared::Messaging::Values &values) {
        this->numberOfPlayers = std::stoi(values[0][0]);

        std::vector<std::vector<Shared::Pod>> &players = this->gameState.getPlayerStates();
        players.clear();
        for (int playerIdx = 0; playerIdx < this->numberOfPlayers; ++playerIdx) {
            std::vector<Shared::Pod> pods;

            for (int podIdx = 0; podIdx < this->settings.getPodsPerPlayer(); ++podIdx) {
                float x = 30.0f;
                float y = 90.0f + (this->settings.getPodsPerPlayer() * playerIdx + podIdx) * 60.0f;
                pods.emplace_back(x, y, 0.0f, 0.0f, 0.0f);
            }

            players.push_back(pods);
        }
    });
}

void Engine::run() {
    Timer timer;
    float dt;

    while (true) {
        dt = timer.elapsed();
        timer.reset();

        int winner = this->update();
        this->writeTurn();
        this->readActions();

        gameState.nextTurn();

        if (winner != -1) {
            std::cerr << "PLAYER #" << winner << " WON!" << std::endl;
            break;
        }
    }
}

void decreaseHpOnCollision(Shared::Physics::Entity* entity){
    Shared::Pod* pod = static_cast<Shared::Pod*>(entity);
    if (pod == nullptr)
        return;
    pod->setHealth(pod->getHealth()-5);

}

int Engine::update() {
    const float TIME = 1.0f;

    float time = 0.0f;
    float mt = TIME;
    Shared::Physics::Entity* podEntity;
    Shared::Physics::Entity* otherEntity;

    while (time +0.01f < TIME) {
        mt = TIME - time;
        podEntity = otherEntity = nullptr;

        for (auto &pods : this->gameState.getPlayerStates()) {
            for (auto &p : pods) {
                float t;

                for (auto &pods2 : this->gameState.getPlayerStates()) {
                    for (auto &o : pods2) {
                        if (&o == &p || o.getHealth() <= 0) {
                            continue;
                        }
                        t = p.collisionTime(o);
                        if (t >= 0.0f && t < mt) {
                            mt = t;
                            podEntity = &p;
                            otherEntity = &o;
                        }
                    }
                }
                for (auto &o : this->settings.getWalls()) {
                    t = p.collisionTime(o);
                    if (t >= 0.0f && t < mt) {
                        mt = t;
                        podEntity = &p;
                        otherEntity = &o;
                    }
                }
            }
        }

        for (auto &pods : this->gameState.getPlayerStates()) {
            for (auto &p : pods) {
                if (p.getHealth() <= 0)
                    continue;
                p.updatePosition(mt - 0.001f);
                p.friction(mt);
                p.checkWin(this->settings.getCheckpoints());
            }
        }

        time += mt;

        if (podEntity != nullptr && otherEntity != nullptr) {
            decreaseHpOnCollision(podEntity);
            decreaseHpOnCollision(otherEntity);
            podEntity->impactRedirection(*otherEntity);
        }
    }

    int winner = -1;
    for (int playerIdx = 0; playerIdx < this->numberOfPlayers; ++playerIdx) {
        for (auto &pod : this->gameState.getPlayerState(static_cast<unsigned long long int>(playerIdx))) {
            if (pod.checkWin(this->settings.getCheckpoints())) {
                winner = playerIdx;
            }
        }
    }

    return winner;
}

void Engine::writeTurn() {
    for (int playerIdx = 0; playerIdx < this->numberOfPlayers; ++playerIdx) {
        messaging.write(gameState.toMessage(playerIdx + 1));
    }
}

void Engine::readActions() {
    for (int playerIdx = 0; playerIdx < this->numberOfPlayers; ++playerIdx) {
        messaging.read("actions " + std::to_string(gameState.getTurn()) + " " + std::to_string(playerIdx + 1), [&](const Shared::Messaging::Values &values) {
            Shared::Action parsedActions = Shared::Action::parse(values);

            for (const auto &action : parsedActions.getActions()) {
                Shared::Pod &pod = this->gameState.getPodState(static_cast<unsigned long long>(playerIdx),
                                                                 static_cast<unsigned long long>(action.pod));

                float rot = std::max(-Shared::Pod::MAX_TURN, std::min(action.rotation, Shared::Pod::MAX_TURN));
                pod.turn(rot);

                float power = std::max(0.0f, std::min(action.throttle, Shared::Pod::MAX_THRUST));
                pod.thrust(power);
            }
        });
    }
}
