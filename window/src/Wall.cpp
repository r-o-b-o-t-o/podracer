#include "Wall.h"
#include "Random.h"

namespace Window {
    Wall::Wall(const TextureLoader &textureLoader, int x, int y, int radius) :
            x(x),
            y(y),
            radius(radius) {

        if (radius <= 12) {
            this->sprite = sf::Sprite(*textureLoader.get("walls/tire"));
        } else {
            int rand = Shared::Random::getInt(1, 3);
            if (rand == 1) {
                this->sprite = sf::Sprite(*textureLoader.get("walls/rock"));
            } else if (rand == 2) {
                this->sprite = sf::Sprite(*textureLoader.get("walls/tree"));
            } else if (rand == 3) {
                this->sprite = sf::Sprite(*textureLoader.get("walls/bush"));
            }
        }

        sf::FloatRect bounds = this->sprite.getLocalBounds();
        this->sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        this->sprite.setPosition(x, y);
        float scale = radius * 2.0f / 64.0f;
        this->sprite.setScale(scale, scale);
    }

    const sf::Sprite &Wall::getSprite() const {
        return this->sprite;
    }
}
