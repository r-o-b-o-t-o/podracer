#include <fstream>

#include "Engine.h"
#include "Timer.h"
#include "Action.h"
#include "Random.h"
#include "Physics.h"

#include "json.hpp"

Engine::Engine(int podsPerPlayer, const std::string &importExport) {
    this->settings.setPodsPerPlayer(podsPerPlayer);
    this->settings.setWidth(1000);
    this->settings.setHeight(800);

    this->readPlayers();
    if (importExport.empty()) {
        this->initRandomMap();
    } else if (importExport == "track") {
        this->initTrack();
    } else {
        this->handleImportExport(importExport);
    }
    this->messaging.write(this->settings.toMessage());
    this->run();
}

void Engine::initRandomMap() {
    using namespace Shared;

    int nbWalls = Random::getInt(3, 6);
    int nbCheckpoints = Random::getInt(2, 4);

    auto getRandomEntity = [&](bool* success) {
        Physics::Entity entity(0.0f, 0.0f, 0.0f);
        int tries = 0;
        int maxTries = 100;
        float border = 120.0f + Pod::RADIUS;

        while (true) {
            float x = Random::getFloat(border, this->settings.getWidth() - border);
            float y = Random::getFloat(border, this->settings.getHeight() - border);
            float radius = Random::getFloat(20.0f, 50.0f);
            entity = Physics::Entity(x, y, radius);
            bool valid = true;

            std::vector<Shared::Physics::Entity> entities;
            entities.insert(entities.end(), this->settings.getCheckpoints().begin(), this->settings.getCheckpoints().end());
            entities.insert(entities.end(), this->settings.getWalls().begin(), this->settings.getWalls().end());

            for (const Physics::Entity &other : entities) {
                if (Shared::Physics::circlesOverlap(
                        x, y, radius + Shared::Pod::RADIUS * 2.0f,
                        other.getX(), other.getY(), other.getRadius() + Shared::Pod::RADIUS * 2.0f
                )) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                *success = true;
                break;
            }

            tries++;
            if (tries > maxTries) {
                *success = false;
                break;
            }
        }

        return entity;
    };

    bool success;
    this->settings.getWalls().clear();
    for (int i = 0; i < nbWalls; ++i) {
        auto entity = getRandomEntity(&success);
        if (success) {
            this->settings.addWall(entity);
        }
    }
    this->settings.getCheckpoints().clear();
    for (int i = 0; i < nbCheckpoints; ++i) {
        auto entity = getRandomEntity(&success);
        if (success) {
            this->settings.addCheckpoint(entity);
        }
    }

    if (this->settings.getCheckpoints().empty()) {
        this->initRandomMap();
    }

    auto &players = this->gameState.getPlayerStates();
    players.clear();
    int side = Random::getInt(0, 3);
    float startX = 0.0f;
    float startY = 0.0f;
    float offset = Pod::RADIUS * 3.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;

    if (side == 0) {
        startX = this->settings.getWidth() / 2.0f;
        startY = 40.0f;
        offsetX = offset;
    } else if (side == 1) {
        startX = this->settings.getWidth() - 40.0f;
        startY = this->settings.getHeight() / 2.0f;
        offsetY = offset;
    } else if (side == 2) {
        startX = this->settings.getWidth() / 2.0f;
        startY = this->settings.getHeight() - 40.0f;
        offsetX = offset;
    } else if (side == 3) {
        startX = 40.0f;
        startY = this->settings.getHeight() / 2.0f;
        offsetY = offset;
    }

    int i = -(this->numberOfPlayers * this->settings.getPodsPerPlayer()) / 2;
    for (int playerIdx = 0; playerIdx < this->numberOfPlayers; ++playerIdx) {
        std::vector<Shared::Pod> pods;

        for (int podIdx = 0; podIdx < this->settings.getPodsPerPlayer(); ++podIdx) {
            float x = startX + i * offsetX;
            float y = startY + i * offsetY;
            pods.emplace_back(x, y, 0.0f, 0.0f, 0.0f);

            i++;
        }

        players.push_back(pods);
    }
}

void Engine::initTrack() {
    this->settings.setWidth(1920);
    this->settings.setHeight(1080);

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

    //Players
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
}

void Engine::readPlayers() {
    this->messaging.read("players", [&](const Shared::Messaging::Values &values) {
        this->numberOfPlayers = std::stoi(values[0][0]);
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
        this->initRandomMap();
        this->exportLevel(file);
    }
}
