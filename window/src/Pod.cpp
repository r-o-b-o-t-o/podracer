#include <algorithm>

#include "Pod.h"
#include "Random.h"
#include "Physics.h"
#include "shared/include/Pod.h"

namespace Window {
    Pod::Pod(const TextureLoader &textureLoader, const FontLoader &fontLoader, int playerIdx, int podIdx) :
            tooltip(textureLoader, fontLoader, playerIdx, podIdx),
            playerIdx(playerIdx),
            podIdx(podIdx) {

        this->model = Pod::getModelId(playerIdx, podIdx);
        for (int i = 1; i <= 4; ++i) {
            const sf::Texture* texture = textureLoader.get("pod_" + std::to_string(this->model) + "/" + std::to_string(i));
            sf::Sprite sprite(*texture);
            sf::FloatRect bounds = sprite.getGlobalBounds();
            sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
            float scale = Shared::Pod::RADIUS * 2.0f / static_cast<float>(texture->getSize().x);
            sprite.setScale(scale, scale);

            this->sprites[i - 1] = sprite;
        }

        this->setHealth(100.0f);
        this->setRotation(0.0f);

        this->debugBounds = sf::CircleShape(Shared::Pod::RADIUS);
        this->debugBounds.setOrigin(Shared::Pod::RADIUS, Shared::Pod::RADIUS);
        this->debugBounds.setOutlineColor(sf::Color::Cyan);
        this->debugBounds.setFillColor(sf::Color::Transparent);
        this->debugBounds.setOutlineThickness(2.0f);
    }

    const sf::Sprite &Pod::getSprite() const {
        return this->sprites[this->currentSprite];
    }

    void Pod::setHealth(float health) {
        if (health < 0.0f) {
            health = 0.0f;
        } else if (health > 100.0f) {
            health = 100.0f;
        }

        int modelsCount = 4;
        this->currentSprite = std::min(static_cast<int>((100.0f - health) / (100.0f / modelsCount)), modelsCount - 1);
        this->tooltip.setHealth(health);
    }

    void Pod::setPosition(int x, int y) {
        this->x = x;
        this->y = y;
        for (sf::Sprite &sprite : this->sprites) {
            sprite.setPosition(x, y);
        }
        this->debugBounds.setPosition(x, y);
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

    void Pod::setNextCheckpoint(int nextCheckpoint) {
        this->tooltip.setNextCheckpoint(nextCheckpoint);
    }

    int Pod::getModelId(int playerIdx, int podIdx) {
        int id = playerIdx % 4; // 4 different colors
        id *= 3; // 3 skins per color
        id += podIdx % 3;
        id++; // 1-based indexing
        return id;
    }

    void Pod::update(const sf::Event &e) {
        this->tooltip.update(e);
        this->tooltip.setVisible(Shared::Physics::containsCenter(this->x, this->y, Shared::Pod::RADIUS, e.mouseMove.x, e.mouseMove.y));
    }

    void Pod::draw(sf::RenderWindow &window) const {
        window.draw(this->getSprite());
    }

    void Pod::debug(sf::RenderWindow &window) const {
        window.draw(this->debugBounds);
    }

    void Pod::drawUi(sf::RenderWindow &window) const {
        this->tooltip.draw(window);
    }
}
