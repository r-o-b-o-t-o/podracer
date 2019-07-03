#include "Pod.h"
#include "Random.h"

namespace Window {
    Pod::Pod(const TextureLoader &textureLoader) {
        this->model = Shared::Random::getInt(1, 3);
        for (int i = 0; i < 5; ++i) {
            const sf::Texture* texture = textureLoader.get(
                    "car_" + std::to_string(this->model) + "/" + std::to_string(i + 1));
            sf::Sprite sprite(*texture);
            sf::FloatRect bounds = sprite.getLocalBounds();
            sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

            this->sprites[i] = sprite;
        }

        this->setHealth(100.0f);
    }

    const sf::Sprite &Pod::getSprite() const {
        return this->sprites[this->currentSprite];
    }

    float Pod::getHealth() const {
        return this->health;
    }

    void Pod::setHealth(float health) {
        if (health < 0.0f) {
            health = 0.0f;
        } else if (health > 100.0f) {
            health = 100.0f;
        }

        this->health = health;

        this->currentSprite = std::min(static_cast<int>((100.0f - health) / 20.0f), 4);
    }

    void Pod::setPosition(int x, int y) {
        for (sf::Sprite &sprite : this->sprites) {
            sprite.setPosition(x, y);
        }
    }

    void Pod::setRotation(float angle) {
        for (sf::Sprite &sprite : this->sprites) {
            sprite.setRotation(angle);
        }
    }
}
