#ifndef WINDOW_POD_H
#define WINDOW_POD_H

#include "Checkpoint.h"
#include "TextureLoader.h"

namespace Window {
    class Pod {
    public:
        explicit Pod(const TextureLoader &textureLoader);

        const sf::Sprite &getSprite() const;
        float getHealth() const;

        void setHealth(float health);
        void setPosition(int x, int y);
        void setRotation(float angle);

        void collisions(std::vector<Checkpoint> &checkpoints);

    private:
        int x;
        int y;
        int model;
        float health;
        int currentSprite;
        sf::Sprite sprites[5];
        std::vector<Checkpoint*> collidingCheckpoints;
    };
}

#endif
