#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <SFML/Graphics.hpp>

namespace Window {
    class FontLoader {
    public:
        FontLoader();
        const sf::Font* get(const std::string &name) const;

    private:
        std::map<std::string, sf::Font> fonts;
    };
}

#endif
