#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <SFML/Graphics.hpp>

class TextureLoader {
public:
    TextureLoader();
    sf::Texture &get(const std::string &name);

private:
    std::map<std::string, sf::Texture> textures;

};

#endif
