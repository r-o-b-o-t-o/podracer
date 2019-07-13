#ifndef POD_TOOLTIP_H
#define POD_TOOLTIP_H

#include "FontLoader.h"
#include "TextureLoader.h"

namespace Window {
    class PodTooltip {
    public:
        PodTooltip(const TextureLoader &textureLoader, const FontLoader &fontLoader,  int playerIdx, int podIdx);

        void show();
        void hide();
        void setVisible(bool v);
        void setNextCheckpoint(int nextCheckpoint);
        void setHealth(float health);
        void update(const sf::Event &e);
        void draw(sf::RenderWindow &window) const;

    private:
        sf::RectangleShape frame;
        sf::RectangleShape healthBarContainer;
        sf::RectangleShape healthBar;
        sf::Text playerInfoText;
        sf::Text checkpointText;
        bool enabled;
    };
}

#endif
