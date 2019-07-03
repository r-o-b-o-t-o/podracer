#include <vector>

#include "TextureLoader.h"

namespace Window {
    TextureLoader::TextureLoader() {
        std::vector<std::string> texturePaths = {
                "car_1/1.png",
                "car_1/2.png",
                "car_1/3.png",
                "car_1/4.png",
                "car_1/5.png",

                "car_2/1.png",
                "car_2/2.png",
                "car_2/3.png",
                "car_2/4.png",
                "car_2/5.png",

                "car_3/1.png",
                "car_3/2.png",
                "car_3/3.png",
                "car_3/4.png",
                "car_3/5.png",

                "walls/tire.png",
                "walls/rock.png",
                "walls/tree.png",
                "walls/bush.png",

                "checkpoint/off.png",
                "checkpoint/on.png",
        };

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
