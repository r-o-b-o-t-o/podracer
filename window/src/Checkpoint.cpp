#include "Checkpoint.h"

namespace Window {
    Checkpoint::Checkpoint(const TextureLoader &textureLoader, int x, int y, int radius) :
            x(x),
            y(y),
            radius(radius),
            currentCollisions(0) {

        this->sprites[0] = sf::Sprite(*textureLoader.get("checkpoint/off"));
        this->sprites[1] = sf::Sprite(*textureLoader.get("checkpoint/on"));

        for (sf::Sprite &sprite : this->sprites) {
            sf::FloatRect bounds = sprite.getLocalBounds();
            sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
            sprite.setPosition(x, y);
            float scale = radius * 2.0f / 1000.0f;
            sprite.setScale(scale, scale);
        }
    }

    const sf::Sprite &Checkpoint::getSprite() const {
        return this->sprites[0];
    }
}
