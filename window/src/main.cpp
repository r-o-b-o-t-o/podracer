#include <iostream>
#include <SFML/Graphics.hpp>

#include "Settings.h"
#include "Messaging.h"
#include "TextureLoader.h"

#include "Action.h"
#include "Turn.h"

int main() {
    Messaging messaging;
    TextureLoader textureLoader;

    sf::RenderWindow window(sf::VideoMode(800, 600), L"Ｐ Ｏ Ｄ 　Ｒ Ａ Ｃ Ｅ Ｒ");
    window.setVerticalSyncEnabled(true);
    sf::Event event {};
    sf::Color clearColor(40, 40, 40);

    sf::Sprite sprite;
    sprite.scale(sf::Vector2f(0.075f, 0.075f));
    sprite.setTexture(textureLoader.get("car_3/01"));
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    sprite.setPosition(100, 100);

    Settings settings;
    messaging.setOnMessageEvent("players", [](Messaging &m, const Messaging::Values &values) {
    });
    messaging.setOnMessageEvent("settings", [&](Messaging &m, const Messaging::Values &values) {
        settings = Settings::parse(values);
        m.output(settings.toMessage());
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

        window.draw(sprite);

        window.display();
    }

    return 0;
}
