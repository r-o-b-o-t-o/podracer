#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <SFML/Graphics.hpp>

namespace Window {
    class TextureLoader {
    public:
        TextureLoader();
        const sf::Texture* get(const std::string &name) const;

    private:
        std::map<std::string, sf::Texture> textures;

    };
}

#endif
