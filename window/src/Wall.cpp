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
    }

    const sf::Sprite &Wall::getSprite() const {
        return this->sprite;
    }
}
