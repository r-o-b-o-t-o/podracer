#include <vector>

#include "FontLoader.h"

namespace Window {
    FontLoader::FontLoader() {
        std::vector<std::string> fontPaths = {
            "kenvector_future_thin.ttf",
        };

        for (const std::string &path : fontPaths) {
            std::string fileName = path.substr(0, path.find_last_of('.'));
            this->fonts[fileName] = sf::Font();
            this->fonts[fileName].loadFromFile("assets/fonts/" + path);
        }
    }

    const sf::Font* FontLoader::get(const std::string &name) const {
        return &this->fonts.at(name);
    }
}
