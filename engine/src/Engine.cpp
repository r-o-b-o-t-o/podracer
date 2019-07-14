#include <fstream>

#include "Engine.h"
#include "Timer.h"
#include "Action.h"

#include "json.hpp"

Engine::Engine(int podsPerPlayer, const std::string &importExport) :
        numberOfPlayers(podsPerPlayer) {

    this->settings.setPodsPerPlayer(2);
    this->settings.setWidth(1920);
    this->settings.setHeight(1080);

    if (importExport.empty()) {
        this->init();
    } else {
        this->handleImportExport(importExport);
    }
    this->readPlayers();
    this->messaging.write(this->settings.toMessage());
    this->run();
}

void Engine::init() {
    //road 01
    for (int i = 1; i <= 24; ++i) {
        this->settings.addWall(Shared::Wall(-500 + i * 50.0f, -150.0f, 25.0f));
        if(i > 7){
            this->settings.addWall(Shared::Wall(-500 + i * 50.0f, 150.0f, 25.0f));
        }
    }

    //road 02
    for (int i = 1; i <= 8; ++i) {
        this->settings.addWall(Shared::Wall(i * 50.0f + 750.0f, -150.0f + i * 50.0f , 25.0f));
    }
    this->settings.addWall(Shared::Wall(750.0f, 150.0f, 25.0f));
    this->settings.addWall(Shared::Wall(800.0f, 200.0f, 25.0f));

    //road 03
    for (int i = 1; i <= 10; ++i) {
        this->settings.addWall(Shared::Wall(1150.0f, 250.0f + i * 50.0f, 25.0f));
        if(i <= 6){
            this->settings.addWall(Shared::Wall(800.0f, 200.0f + i * 50.0f, 25.0f));
        }
    }

    //road 04
    for (int i = 1; i <= 33; ++i) {
        this->settings.addWall(Shared::Wall(1150.0f - i * 50.0f, 800.0f, 25.0f));
        if(i <= 6){
            this->settings.addWall(Shared::Wall(800.0f - i * 50.f, 500.0f, 25.0f));
        }
    }

    //barrier 01
    for (int i = 1; i <= 3; ++i) {
        this->settings.addWall(Shared::Wall(250.0f, 800.0f - i * 50.0f , 25.0f));
    }

    //road 05
    for (int i = 1; i <= 3; ++i) {
        this->settings.addWall(Shared::Wall(500.0f - i * 50.0f, 550.0f - i * 50.0f , 25.0f));
    }

    //road 06
    for (int i = 1; i <= 4; ++i) {
        this->settings.addWall(Shared::Wall(350.0f - i * 50.0f, 400.0f, 25.0f));
    }

    //road 07
    for (int i = 1; i <= 3; ++i) {
        this->settings.addWall(Shared::Wall(150.0f - i * 50.0f, 400.0f + i * 50.0f , 25.0f));
    }

    //road 08
    for (int i = 1; i <= 3; ++i) {
        this->settings.addWall(Shared::Wall(0.0f - i * 50.0f, 550.0f, 25.0f));
    }

    //road 09
    for (int i = 1; i <= 19; ++i) {
        this->settings.addWall(Shared::Wall(-500.0f, 800.0f - i * 50.0f, 25.0f));
        if(i >= 6 && i <= 13){
            this->settings.addWall(Shared::Wall(-150.0f, 800.0f - i * 50.0f, 25.0f));
        }
    }

    //Checkpoints
    this->settings.addCheckpoint(Shared::Checkpoint(700.0f, 0.0f, 48.0f));
    this->settings.addCheckpoint(Shared::Checkpoint(975.0f, 200.0f, 36.0f));
    this->settings.addCheckpoint(Shared::Checkpoint(975.0f, 650.0f, 36.0f));
    this->settings.addCheckpoint(Shared::Checkpoint(500.0f, 650.0f, 36.0f));
    this->settings.addCheckpoint(Shared::Checkpoint(250.0f, 500.0f, 36.0f));
    this->settings.addCheckpoint(Shared::Checkpoint(50.0f, 650.0f, 36.0f));
    this->settings.addCheckpoint(Shared::Checkpoint(-325.0f, 650.0f, 36.0f));
    this->settings.addCheckpoint(Shared::Checkpoint(-325.0f, 0.0f, 36.0f));
}

void Engine::readPlayers() {
    this->messaging.read("players", [&](const Shared::Messaging::Values &values) {
        this->numberOfPlayers = std::stoi(values[0][0]);

        std::vector<std::vector<Shared::Pod>> &players = this->gameState.getPlayerStates();
        players.clear();
        for (int playerIdx = 0; playerIdx < this->numberOfPlayers; ++playerIdx) {
            std::vector<Shared::Pod> pods;

            for (int podIdx = 0; podIdx < this->settings.getPodsPerPlayer(); ++podIdx) {
                float x = -200.0f + (this->settings.getPodsPerPlayer() * podIdx) * 30.0f;
                float y = -30.0f + (this->settings.getPodsPerPlayer() * playerIdx) * 30.0f;
                pods.emplace_back(x, y, 0.0f, 0.0f, 0.0f);
            }

            players.push_back(pods);
        }
    });
}

void Engine::run() {
    Timer timer;
    float dt;
    int winner = -1;
    this->damageEnabled = true;

    while (true) {
        dt = timer.elapsed();
        timer.reset();

        if (winner == -1) {
            winner = this->update();
        }
        this->writeTurn();
        this->readActions();

        gameState.nextTurn();
    }
}

void decreaseHpOnCollision(Shared::Physics::Entity* entity){
    Shared::Pod* pod = static_cast<Shared::Pod*>(entity);
    if (pod == nullptr) {
        return;
    }
    pod->setHealth(pod->getHealth() - 1);
}

int Engine::update() {
    const float TIME = 1.0f;

    float time = 0.0f;
    float mt = TIME;
    Shared::Physics::Entity* podEntity;
    Shared::Physics::Entity* otherEntity;

    while (time + 0.01f < TIME) {
        mt = TIME - time;
        podEntity = otherEntity = nullptr;

        for (auto &pods : this->gameState.getPlayerStates()) {
            for (auto &p : pods) {
                float t;

                for (auto &pods2 : this->gameState.getPlayerStates()) {
                    for (auto &o : pods2) {
                        if (&o == &p || o.getHealth() <= 0 || p.getHealth() <= 0) {
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
            if (this->damageEnabled) {
                decreaseHpOnCollision(podEntity);
                decreaseHpOnCollision(otherEntity);
            }
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

void Engine::importLevel(const std::string &file) {
    using namespace nlohmann;

    std::ifstream f(file);
    json j;
    f >> j;

    auto &checkpoints = this->settings.getCheckpoints();
    checkpoints.clear();
    for (const auto &cp : j["checkpoints"]) {
        checkpoints.emplace_back(cp["x"], cp["y"], cp["radius"]);
    }

    auto &walls = this->settings.getWalls();
    walls.clear();
    for (const auto &wall : j["walls"]) {
        walls.emplace_back(wall["x"], wall["y"], wall["radius"]);
    }
}

void Engine::exportLevel(const std::string &file) {
    using namespace nlohmann;

    json j = {
        { "checkpoints", json::array() },
        { "walls", json::array() },
    };

    for (const auto &cp : this->settings.getCheckpoints()) {
        j["checkpoints"].push_back({
            { "x", cp.getX() },
            { "y", cp.getY() },
            { "radius", cp.getRadius() },
        });
    }
    for (const auto &wall : this->settings.getWalls()) {
        j["walls"].push_back({
            { "x", wall.getX() },
            { "y", wall.getY() },
            { "radius", wall.getRadius() },
        });
    }

    std::ofstream f(file);
    f << j << std::endl;
}

void Engine::handleImportExport(std::string file) {
    char action = file[0];
    file.erase(0, 1);

    if (action == '<') {
        this->importLevel(file);
    } else if (action == '>') {
        this->init();
        this->exportLevel(file);
    }
}
