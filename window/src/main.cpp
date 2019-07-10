#include <vector>
#include <SFML/Graphics.hpp>

#include "Pod.h"
#include "Wall.h"
#include "Turn.h"
#include "Settings.h"
#include "Messaging.h"

using namespace Window;

int main() {
    Shared::Messaging messaging;
    TextureLoader textureLoader;
    FontLoader fontLoader;

    sf::RenderWindow window(sf::VideoMode(800, 600), L"Ｐ Ｏ Ｄ 　Ｒ Ａ Ｃ Ｅ Ｒ");
    window.setVerticalSyncEnabled(true);
    sf::Event event {};
    sf::Color clearColor(40, 40, 40);

    Shared::Settings settings;
    int numberOfPlayers = -1;
    std::vector<Pod> pods;
    std::vector<Wall> walls;
    std::vector<Checkpoint> checkpoints;

    messaging.read("settings", [&](const Shared::Messaging::Values &values, const std::smatch &match) {
        settings = Shared::Settings::parse(values);
        window.setSize(sf::Vector2u(static_cast<unsigned int>(settings.getWidth()),
                                    static_cast<unsigned int>(settings.getHeight())));
    });
    messaging.read("players", [&](const Shared::Messaging::Values &values, const std::smatch &match) {
        numberOfPlayers = std::stoi(values[0][0]);
        int numberOfPods = numberOfPlayers * settings.getPodsPerPlayer();

        pods.clear();
        for (int i = 0; i < numberOfPods; ++i) {
            pods.emplace_back(textureLoader);
        }

        walls.clear();
        for (const Shared::Wall &wall : settings.getWalls()) {
            walls.emplace_back(textureLoader, wall.getX(), wall.getY(), wall.getRadius());
        }

        int i = 0;
        checkpoints.clear();
        for (const Shared::Checkpoint &checkpoint : settings.getCheckpoints()) {
            checkpoints.emplace_back(fontLoader, i, checkpoint.getX(), checkpoint.getY(), checkpoint.getRadius());
            ++i;
        }
    });

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

        for (const Checkpoint &checkpoint : checkpoints) {
            checkpoint.draw(window);
        }
        for (const Pod &pod : pods) {
            window.draw(pod.getSprite());
        }
        for (const Wall &wall : walls) {
            window.draw(wall.getSprite());
        }

        window.display();

        messaging.read("turn", [&](const Shared::Messaging::Values &values, const std::smatch &match) {
            Shared::Turn turn = Shared::Turn::parse(values);

            int playerIdx = 0;
            for (auto &playerState : turn.getPlayerStates()) {
                int podIdx = 0;
                for (const Shared::Pod &podState : playerState) {
                    Pod &pod = pods[settings.getPodsPerPlayer() * playerIdx + podIdx];
                    pod.setHealth(podState.getHealth());
                    pod.setPosition(static_cast<int>(podState.getX()), static_cast<int>(podState.getY()));
                    pod.setRotation(podState.getDirection());
                    pod.collisions(checkpoints);

                    ++podIdx;
                }
                ++playerIdx;
            }
        });
    }

    return 0;
}
