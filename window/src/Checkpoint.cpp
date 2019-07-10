#include <cmath>

#include "Checkpoint.h"

namespace Window {
    sf::Color Checkpoint::disabledColor = sf::Color(216, 57, 36);
    sf::Color Checkpoint::enabledColor = sf::Color(29, 214, 33);

    Checkpoint::Checkpoint(const FontLoader &fontLoader, int idx, int x, int y, int radius) :
            x(x),
            y(y),
            radius(radius),
            currentCollisions(0) {

        this->shape = sf::CircleShape(radius, 8);
        this->shape.setOutlineThickness(radius / 16.0f);
        this->applyCollisionEffect();
        this->shape.setFillColor(sf::Color::Transparent);
        sf::FloatRect bounds = this->shape.getLocalBounds();
        this->shape.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        this->shape.setPosition(x, y);

        this->text.setFont(*fontLoader.get("kenvector_future_thin"));
        std::string textContent = "#" + std::to_string(idx);
        this->text.setString(textContent);
        this->text.setCharacterSize(static_cast<unsigned int>(std::round(radius * 0.75f)));
        this->text.setFillColor(sf::Color::White);
        bounds = this->shape.getGlobalBounds();
        sf::FloatRect textBounds = this->text.getGlobalBounds();
        this->text.setPosition(bounds.left + (bounds.width - textBounds.width) / 2.0f,
                               bounds.top + (bounds.height - textBounds.height) / 2.0f - text.getLocalBounds().top);
    }

    void Checkpoint::draw(sf::RenderWindow &window) const {
        window.draw(this->shape);
        window.draw(this->text);
    }

    void Checkpoint::collisionEnter() {
        this->currentCollisions++;
        this->applyCollisionEffect();
    }

    void Checkpoint::collisionExit() {
        this->currentCollisions--;
        this->applyCollisionEffect();
    }

    void Checkpoint::applyCollisionEffect() {
        this->shape.setOutlineColor(this->currentCollisions == 0 ?
                                    Checkpoint::disabledColor : Checkpoint::enabledColor);
    }

    int Checkpoint::getX() const {
        return this->x;
    }

    int Checkpoint::getY() const {
        return this->y;
    }

    int Checkpoint::getRadius() const {
        return this->radius;
    }
}
