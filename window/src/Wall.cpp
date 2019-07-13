#include "Wall.h"
#include "Random.h"

namespace Window {
    Wall::Wall(const TextureLoader &textureLoader, int x, int y, int radius) :
            x(x),
            y(y),
            radius(radius) {

        int type = Shared::Random::getInt(1, 2);
        int color = Shared::Random::getInt(1, 3);
        if (type == 1) {
            this->sprite = sf::Sprite(*textureLoader.get("walls/brown" + std::to_string(color)));
        } else if (type == 2) {
            this->sprite = sf::Sprite(*textureLoader.get("walls/grey" + std::to_string(color)));
        }

        sf::FloatRect bounds = this->sprite.getLocalBounds();
        this->sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        this->sprite.setPosition(x, y);
        float scale = radius * 2.0f / static_cast<float>(this->sprite.getTexture()->getSize().x);
        this->sprite.setScale(scale, scale);

        this->debugBounds = sf::CircleShape(radius);
        this->debugBounds.setPosition(x, y);
        this->debugBounds.setOrigin(radius, radius);
        this->debugBounds.setOutlineColor(sf::Color::Cyan);
        this->debugBounds.setFillColor(sf::Color::Transparent);
        this->debugBounds.setOutlineThickness(2.0f);
    }

    void Wall::draw(sf::RenderWindow &window) const {
        window.draw(this->sprite);
    }

    void Wall::debug(sf::RenderWindow &window) const {
        window.draw(this->debugBounds);
    }
}
