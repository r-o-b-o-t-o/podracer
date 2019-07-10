#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "FontLoader.h"

namespace Window {
    class Checkpoint {
    public:
        explicit Checkpoint(const FontLoader &fontLoader, int idx, int x, int y, int radius);

        void draw(sf::RenderWindow &window) const;

        void collisionEnter();
        void collisionExit();

        int getX() const;
        int getY() const;
        int getRadius() const;

    private:
        int x;
        int y;
        int radius;
        int currentCollisions;

        sf::Text text;
        sf::CircleShape shape;
        static sf::Color enabledColor;
        static sf::Color disabledColor;

        void applyCollisionEffect();
    };
}

#endif
