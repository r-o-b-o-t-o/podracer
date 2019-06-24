#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), L"Ｐ Ｏ Ｄ 　Ｒ Ａ Ｃ Ｅ Ｒ");
    window.setVerticalSyncEnabled(true);
    sf::Event event {};
    sf::Color clearColor(40, 40, 40);

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

        // Draw here

        window.display();
    }

    return 0;
}
