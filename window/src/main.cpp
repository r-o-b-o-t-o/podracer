#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Pod.h"
#include "Turn.h"
#include "Wall.h"
#include "Settings.h"
#include "Checkpoint.h"
#include "TextureLoader.h"

using namespace Window;

int main() {
    Shared::Messaging messaging;
    TextureLoader textureLoader;

    sf::RenderWindow window(sf::VideoMode(800, 600), L"Ｐ Ｏ Ｄ 　Ｒ Ａ Ｃ Ｅ Ｒ");
    window.setVerticalSyncEnabled(true);
    sf::Event event {};
    sf::Color clearColor(40, 40, 40);

    Shared::Settings settings;
    int numberOfPlayers = -1;
    bool loadedSettings = false;
    std::vector<Pod> pods;
    std::vector<Wall> walls;
    std::vector<Checkpoint> checkpoints;

    auto start = [&]() {
        int numberOfPods = numberOfPlayers * settings.getPodsPerPlayer();
        pods.clear();

        for (int i = 0; i < numberOfPods; ++i) {
            pods.emplace_back(textureLoader);
        }

        for (const Shared::Wall &wall : settings.getWalls()) {
            walls.emplace_back(textureLoader, wall.centerX, wall.centerY, wall.radius);
        }
        for (const Shared::Checkpoint &checkpoint : settings.getCheckpoints()) {
            checkpoints.emplace_back(textureLoader, checkpoint.centerX, checkpoint.centerY, checkpoint.radius);
        }
    };

    messaging.setOnMessageEvent("settings", [&](Shared::Messaging &m, const Shared::Messaging::Values &values) {
        settings = Shared::Settings::parse(values);
        window.setSize(sf::Vector2u(static_cast<unsigned int>(settings.getWidth()),
                                    static_cast<unsigned int>(settings.getHeight())));
        loadedSettings = true;

        if (numberOfPlayers != -1) {
            start();
        }
    });
    messaging.setOnMessageEvent("players", [&](Shared::Messaging &m, const Shared::Messaging::Values &values) {
        numberOfPlayers = std::stoi(values[0][0]);
        if (loadedSettings) {
            start();
        }
    });
    messaging.setOnMessageEvent("turn", [&](Shared::Messaging &m, const Shared::Messaging::Values &values) {
        Shared::Turn turn = Shared::Turn::parse(values);

        int playerIdx = 0;
        for (auto &playerState : turn.getPlayerStates()) {
            int podIdx = 0;
            for (const Shared::State &podState : playerState) {
                Pod &pod = pods[playerIdx * podIdx];
                pod.setHealth(podState.health);
                pod.setPosition(podState.x, podState.y);
                pod.setRotation(podState.direction);

                ++podIdx;
            }
            ++playerIdx;
        }
    });
    messaging.start();

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            } else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Key::Escape) {
                    window.close();
                    break;
                }
            }
        }

        window.clear(clearColor);

        for (const Pod &pod : pods) {
            window.draw(pod.getSprite());
        }
        for (const Wall &wall : walls) {
            window.draw(wall.getSprite());
        }
        for (const Checkpoint &checkpoint : checkpoints) {
            window.draw(checkpoint.getSprite());
        }

        window.display();
    }

    return 0;
}
