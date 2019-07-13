#include "PodTooltip.h"

namespace Window {
    PodTooltip::PodTooltip(const Window::TextureLoader &textureLoader, const Window::FontLoader &fontLoader, int playerIdx, int podIdx) :
            enabled(false) {

        this->frame = sf::RectangleShape(sf::Vector2f(210.0f, 64.0f));
        this->frame.setFillColor(sf::Color(0, 0, 0, 70));

        this->healthBarContainer = sf::RectangleShape(sf::Vector2f(100.0f, 16.0f));
        this->healthBarContainer.setFillColor(sf::Color(224, 51, 51, 50));

        this->healthBar = sf::RectangleShape(sf::Vector2f(100.0f, 16.0f));
        this->healthBar.setFillColor(sf::Color(24, 226, 45, 200));

        this->playerInfoText = sf::Text("P" + std::to_string(playerIdx + 1) + " / #" + std::to_string(podIdx + 1), *fontLoader.get("kenvector_future_thin"), 16);
        this->playerInfoText.setFillColor(sf::Color::White);

        this->checkpointText = sf::Text("", *fontLoader.get("kenvector_future_thin"), 16);
        this->checkpointText.setFillColor(sf::Color::White);
    }

    void PodTooltip::show() {
        this->setVisible(true);
    }

    void PodTooltip::hide() {
        this->setVisible(false);
    }

    void PodTooltip::setVisible(bool v) {
        this->enabled = v;
    }

    void PodTooltip::setNextCheckpoint(int nextCheckpoint) {
        this->checkpointText.setString("Next checkpoint: #" + std::to_string(nextCheckpoint));
    }

    void PodTooltip::setHealth(float health) {
        float height = this->healthBar.getSize().y;
        this->healthBar.setSize(sf::Vector2f(health, height));
    }

    void PodTooltip::update(const sf::Event &e) {
        if (e.type == sf::Event::EventType::MouseMoved) {
            int x = e.mouseMove.x + 16;
            int y = e.mouseMove.y + 16;
            this->frame.setPosition(x, y);
            this->playerInfoText.setPosition(x + 12, y + 10);
            this->checkpointText.setPosition(x + 12, y + 32);
            this->healthBarContainer.setPosition(x + 86, y + 12);
            this->healthBar.setPosition(x + 86, y + 12);
        }
    }

    void PodTooltip::draw(sf::RenderWindow &window) const {
        if (!this->enabled) {
            return;
        }

        window.draw(this->frame);
        window.draw(this->playerInfoText);
        window.draw(this->checkpointText);
        window.draw(this->healthBarContainer);
        window.draw(this->healthBar);
    }
}
