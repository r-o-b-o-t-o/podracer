#ifndef WINDOW_POD_H
#define WINDOW_POD_H

#include <SFML/Window/Event.hpp>

#include "Checkpoint.h"
#include "FontLoader.h"
#include "TextureLoader.h"
#include "PodTooltip.h"

namespace Window {
    class Pod {
    public:
        Pod(const TextureLoader &textureLoader, const FontLoader &fontLoader, int playerIdx, int podIdx);

        void setHealth(float health);
        void setPosition(int x, int y);
        void setRotation(float angle);

        void collisions(std::vector<Checkpoint> &checkpoints);
        void setNextCheckpoint(int nextCheckpoint);

        void update(const sf::Event &e);
        void draw(sf::RenderWindow &window) const;
        void drawUi(sf::RenderWindow &window) const;
        void debug(sf::RenderWindow &window) const;

    private:
        int x;
        int y;
        int playerIdx;
        int podIdx;
        int model;
        int currentSprite;
        sf::Sprite sprites[5];
        std::vector<Checkpoint*> collidingCheckpoints;
        PodTooltip tooltip;

        const sf::Sprite &getSprite() const;
        static int getModelId(int playerIdx, int podIdx);
        sf::CircleShape debugBounds;
    };
}

#endif
