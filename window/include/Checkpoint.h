#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include "TextureLoader.h"

namespace Window {
    class Checkpoint {
    public:
        explicit Checkpoint(const TextureLoader &textureLoader, int x, int y, int radius);

        const sf::Sprite &getSprite() const;

    private:
        int x;
        int y;
        int radius;
        int currentCollisions;

        sf::Sprite sprites[2];
    };
}

#endif
