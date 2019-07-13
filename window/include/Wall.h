#ifndef WALL_H
#define WALL_H

#include "TextureLoader.h"

namespace Window {
    class Wall {
    public:
        explicit Wall(const TextureLoader &textureLoader, int x, int y, int radius);

        void draw(sf::RenderWindow &window) const;
        void debug(sf::RenderWindow &window) const;

    private:
        int x;
        int y;
        int radius;

        sf::Sprite sprite;
        sf::CircleShape debugBounds;
    };
}

#endif
