#ifndef WALL_H
#define WALL_H

#include "TextureLoader.h"

namespace Window {
    class Wall {
    public:
        explicit Wall(const TextureLoader &textureLoader, int x, int y, int radius);

        const sf::Sprite &getSprite() const;

    private:
        int x;
        int y;
        int radius;

        sf::Sprite sprite;

    };
}

#endif
