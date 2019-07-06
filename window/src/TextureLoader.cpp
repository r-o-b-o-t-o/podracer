#include <vector>

#include "TextureLoader.h"

namespace Window {
    TextureLoader::TextureLoader() {
        std::vector<std::string> texturePaths = {
        };

        for (int i = 1; i <= 3; ++i) {
            texturePaths.push_back("walls/brown" + std::to_string(i) + ".png");
            texturePaths.push_back("walls/grey" + std::to_string(i) + ".png");
        }

        for (int i = 1; i <= 12; ++i) {
            for (int j = 1; j <= 4; ++j) {
                texturePaths.push_back("pod_" + std::to_string(i) + "/" + std::to_string(j) + ".png");
            }
        }

        for (const std::string &path : texturePaths) {
            std::string fileName = path.substr(0, path.find_last_of('.'));
            this->textures[fileName] = sf::Texture();
            this->textures[fileName].setSmooth(true);
            this->textures[fileName].loadFromFile("assets/textures/" + path);
        }
    }

    const sf::Texture* TextureLoader::get(const std::string &name) const {
        return &this->textures.at(name);
    }
}
