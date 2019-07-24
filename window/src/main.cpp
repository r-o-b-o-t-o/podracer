#include <vector>

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

    sf::RenderWindow window(sf::VideoMode(1000, 800), L"Ｐ Ｏ Ｄ 　Ｒ Ａ Ｃ Ｅ Ｒ");
    window.setVerticalSyncEnabled(true);
    sf::Event event {};
    sf::Color clearColor(40, 40, 40);
    bool debug = false;

    Shared::Settings settings;
    std::vector<Pod> pods;
    std::vector<Wall> walls;
    std::vector<Checkpoint> checkpoints;

    messaging.read("settings", [&](const Shared::Messaging::Values &values) {
        settings = Shared::Settings::parse(values);
        window.setSize(sf::Vector2u(static_cast<unsigned int>(settings.getWidth()),
                                    static_cast<unsigned int>(settings.getHeight())));
    });
    messaging.read("players", [&](const Shared::Messaging::Values &values) {
        int numberOfPlayers = std::stoi(values[0][0]);

        pods.clear();
        for (int i = 0; i < numberOfPlayers; ++i) {
            for (int j = 0; j < settings.getPodsPerPlayer(); ++j) {
                pods.emplace_back(textureLoader, fontLoader, i, j);
            }
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

    Shared::Turn turn;
    int winner = -1;
    sf::Text winnerText("", *fontLoader.get("kenvector_future_thin"), 64);
    winnerText.setOutlineThickness(4.0f);
    winnerText.setOutlineColor(sf::Color(30, 30, 30));

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                window.setView(sf::View(window.getView().getCenter(), sf::Vector2f(event.size.width, event.size.height)));
            } else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Key::Escape) {
                    window.close();
                    break;
                }
                if (event.key.code == sf::Keyboard::Key::D) {
                    debug = !debug;
                    break;
                }
            } else if (event.type == sf::Event::MouseMoved) {
                for (Pod &pod : pods) {
                    pod.update(event);
                }
            }
        }

        window.clear(clearColor);

        for (const Checkpoint &checkpoint : checkpoints) {
            checkpoint.draw(window);
            if (debug) {
                checkpoint.debug(window);
            }
        }
        for (const Pod &pod : pods) {
            pod.draw(window);
            if (debug) {
                pod.debug(window);
            }
        }
        for (const Wall &wall : walls) {
            wall.draw(window);
            if (debug) {
                wall.debug(window);
            }
        }
        for (const Pod &pod : pods) {
            pod.drawUi(window);
        }
        if (winner != -1) {
            window.draw(winnerText);
        }

        window.display();

        messaging.read("turn", [&](const Shared::Messaging::Values &values) {
            turn.update(values);

            int playerIdx = 0;
            for (auto &playerState : turn.getPlayerStates()) {
                int podIdx = 0;
                for (Shared::Pod &podState : playerState) {
                    if (winner == -1 && podState.checkWin(settings.getCheckpoints())) {
                        winner = playerIdx;
                        winnerText.setString("PLAYER " + std::to_string(winner + 1) + " WON!");
                        sf::FloatRect bounds = winnerText.getLocalBounds();
                        winnerText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
                        const auto &center = window.getView().getCenter();
                        winnerText.setPosition(center);
                    }

                    Pod &pod = pods[settings.getPodsPerPlayer() * playerIdx + podIdx];
                    pod.setHealth(podState.getHealth());
                    pod.setPosition(static_cast<int>(podState.getX()), static_cast<int>(podState.getY()));
                    pod.setRotation(podState.getDirection());
                    pod.collisions(checkpoints);
                    pod.setNextCheckpoint(podState.getNextCheckpoint());
                    pod.setHealth(podState.getHealth());

                    ++podIdx;
                }
                ++playerIdx;
            }
        });
    }

    return 0;
}
