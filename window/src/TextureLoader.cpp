#include <vector>

#include "TextureLoader.h"

TextureLoader::TextureLoader() {
    std::vector<std::string> texturePaths = {
        "car_3/01.png",
        "car_3/02.png",
        "car_3/03.png",
        "car_3/04.png",
        "car_3/05.png",
    };

    for (const std::string &path : texturePaths) {
        std::string fileName = path.substr(0, path.find_last_of('.'));
        this->textures[fileName] = sf::Texture();
        this->textures[fileName].setSmooth(true);
        this->textures[fileName].loadFromFile("assets/textures/" + path);
    }
}

sf::Texture &TextureLoader::get(const std::string &name) {
    return this->textures.at(name);
}
