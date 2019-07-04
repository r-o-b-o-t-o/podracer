#include <chrono>
#include <thread>

#include "Messaging.h"
#include "Settings.h"
#include "Turn.h"

enum GameStep {
    waiting,
    starting,
    playing,
};

int main(int argc, char** argv) {
    Shared::Messaging m;

    bool run = true;
    int numberOfPlayers;
    GameStep gameStep = GameStep::waiting;

    Shared::Turn gameState;

    Shared::Settings settings;
    settings.setPodsPerPlayer(argc > 1 ? std::stoi(argv[1]) : 1);
    settings.setWidth(800);
    settings.setHeight(600);
    for (int i = 0; i < 5; ++i) {
        Shared::Wall wall {};
        wall.radius = 32;
        wall.centerX = 50 + wall.radius * 2 * i;
        wall.centerY = 200;
        settings.addWall(wall);
    }
    for (int i = 0; i < 5; ++i) {
        Shared::Wall wall {};
        wall.radius = 12;
        wall.centerX = 50 + wall.radius * 2 * i;
        wall.centerY = 400;
        settings.addWall(wall);
    }
    {
        Shared::Checkpoint checkpoint {};
        checkpoint.radius = 64;
        checkpoint.centerX = 500;
        checkpoint.centerY = 400;
        settings.addCheckpoint(checkpoint);
    }

    m.setOnMessageEvent("players", [&](Shared::Messaging &m, const Shared::Messaging::Values &values, const std::smatch &match) {
        numberOfPlayers = std::stoi(values[0][0]);
        m.output(settings.toMessage());
        gameStep = GameStep::playing;

        std::vector<std::vector<Shared::State>> &players = gameState.getPlayerStates();
        players.clear();
        for (int playerIdx = 0; playerIdx < numberOfPlayers; ++playerIdx) {
            std::vector<Shared::State> pods;

            for (int podIdx = 0; podIdx < settings.getPodsPerPlayer(); ++podIdx) {
                Shared::State state {};
                state.direction = -45.0f;
                state.health = 100.0f;
                state.x = static_cast<int>(30.0f);
                state.y = static_cast<int>(30.0f + (playerIdx + podIdx) * 20.0f);
                pods.push_back(state);
            }

            players.push_back(pods);
        }
    });

    m.setOnMessageEvent("shutdown", [&run](Shared::Messaging &m, const Shared::Messaging::Values &values, const std::smatch &match) {
        run = false;
        m.stop();
    });

    m.start();

    using clock = std::chrono::high_resolution_clock;
    auto startTime = clock::now();
    typedef std::chrono::duration<double, std::ratio<1, 60>> ticks;

    while (run) {
        auto now = clock::now();
        auto deltaTime = now - startTime;
        startTime = now;

        if (gameStep == GameStep::waiting) {
            std::this_thread::sleep_for(ticks(1));
            continue;
        }

        for (int playerIdx = 0; playerIdx < numberOfPlayers; ++playerIdx) {
            for (auto &pod : gameState.getPlayerState(static_cast<unsigned long long int>(playerIdx))) {
                pod.x += 0.05f;
            }
            m.output(gameState.toMessage(playerIdx + 1));
        }
        gameState.nextTurn();

        std::this_thread::sleep_for(ticks(1));
    }

    return 0;
}
