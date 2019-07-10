#include <algorithm>

#include "Pod.h"
#include "Random.h"
#include "Physics.h"
#include "shared/include/Pod.h"

namespace Window {
    Pod::Pod(const TextureLoader &textureLoader) {
        this->model = Shared::Random::getInt(1, 12);
        for (int i = 1; i <= 4; ++i) {
            const sf::Texture* texture = textureLoader.get("pod_" + std::to_string(this->model) + "/" + std::to_string(i));
            sf::Sprite sprite(*texture);
            sf::FloatRect bounds = sprite.getLocalBounds();
            sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
            sprite.setScale(0.5f, 0.5f);

            this->sprites[i - 1] = sprite;
        }

        this->setHealth(100.0f);
        this->setRotation(0.0f);
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

        int modelsCount = 4;
        this->currentSprite = std::min(static_cast<int>((100.0f - health) / (100.0f / modelsCount)), modelsCount - 1);
    }

    void Pod::setPosition(int x, int y) {
        this->x = x;
        this->y = y;
        for (sf::Sprite &sprite : this->sprites) {
            sprite.setPosition(x, y);
        }
    }

    void Pod::setRotation(float angle) {
        for (sf::Sprite &sprite : this->sprites) {
            sprite.setRotation(angle + 90.0f);
        }
    }

    void Pod::collisions(std::vector<Checkpoint> &checkpoints) {
        for (Checkpoint &cp : checkpoints) {
            bool alreadyCollides = false;
            for (Checkpoint* ptr : this->collidingCheckpoints) {
                if (ptr == &cp) {
                    alreadyCollides = true;
                    break;
                }
            }

            if (Shared::Physics::containsCenter(cp.getX(), cp.getY(), cp.getRadius(), this->x, this->y)) {
                if (!alreadyCollides) {
                    this->collidingCheckpoints.push_back(&cp);
                    cp.collisionEnter();
                }
            } else {
                if (alreadyCollides) {
                    this->collidingCheckpoints.erase(std::remove(this->collidingCheckpoints.begin(), this->collidingCheckpoints.end(), &cp), this->collidingCheckpoints.end());
                    cp.collisionExit();
                }
            }
        }
    }
}
