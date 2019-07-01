#include <sstream>

#include "Settings.h"

Settings Settings::parse(const Messaging::Values &values) {
    Settings settings {};
    for (auto it = values.begin(); it < values.end(); ++it) {
        auto line = *it;

        if (line[0] == "NB_PODS") {
            settings.podsPerPlayer = std::stoi(line[1]);
        } else if (line[0] == "DIMENSIONS") {
            settings.width = std::stoi(line[1]);
            settings.height = std::stoi(line[2]);
        } else if (line[0] == "WALLS") {
            int n = std::stoi(line[1]);
            std::vector<Wall> walls;

            for (int i = 0; i < n; ++i) {
                ++it;
                line = *it;
                walls.push_back(Wall {
                        std::stoi(line[0]),
                        std::stoi(line[1]),
                        std::stoi(line[2])
                });
            }
            settings.walls = walls;
        } else if (line[0] == "CHECKPOINTS") {
            int n = std::stoi(line[1]);
            std::vector<Checkpoint> checkpoints;

            for (int i = 0; i < n; ++i) {
                ++it;
                line = *it;
                checkpoints.push_back(Checkpoint {
                        std::stoi(line[0]),
                        std::stoi(line[1]),
                        std::stoi(line[2])
                });
            }
            settings.checkpoints = checkpoints;
        }
    }

    return settings;
}

Message Settings::toMessage() {
    Message m("settings");
    std::stringstream stream;

    m.addValue("NB_PODS " + std::to_string(this->podsPerPlayer));
    m.addValue("DIMENSIONS " + std::to_string(this->width) + " " + std::to_string(this->height));
    m.addValue("WALLS " + std::to_string(this->walls.size()));
    for (const Wall &wall : this->walls) {
        m.addValue(std::to_string(wall.centerX) + " " + std::to_string(wall.centerY) + " " + std::to_string(wall.radius));
    }
    m.addValue("CHECKPOINTS " + std::to_string(this->checkpoints.size()));
    for (const Checkpoint &checkpoint : this->checkpoints) {
        m.addValue(std::to_string(checkpoint.centerX) + " " + std::to_string(checkpoint.centerY) + " " + std::to_string(checkpoint.radius));
    }

    return m;
}

std::ostream &operator<<(std::ostream &stream, const Settings &settings) {
    stream << "Settings { " << std::endl;
    stream << "    Pods Per Player: " << settings.podsPerPlayer << "," << std::endl;
    stream << "    Dimensions: " << settings.width << " x " << settings.height << "," << std::endl;
    stream << "    Walls: [" << std::endl;
    for (auto it = settings.walls.begin(); it != settings.walls.end(); ++it) {
        const Wall &wall = *it;
        stream << "        Wall { x: " << wall.centerX << ", y: " << wall.centerY << ", radius: " << wall.radius << " }";
        if (it + 1 != settings.walls.end()) {
            stream << ",";
        }
        stream << std::endl;
    }
    stream << "    ]," << std::endl;
    stream << "    Checkpoints: [" << std::endl;
    for (auto it = settings.checkpoints.begin(); it != settings.checkpoints.end(); ++it) {
        const Checkpoint &checkpoint = *it;
        stream << "        Checkpoint { x: " << checkpoint.centerX << ", y: " << checkpoint.centerY << ", radius: " << checkpoint.radius << " }";
        if (it + 1 != settings.checkpoints.end()) {
            stream << ",";
        }
        stream << std::endl;
    }
    stream << "    ]" << std::endl;
    stream << "}";

    return stream;
}

int Settings::getPodsPerPlayer() const {
    return this->podsPerPlayer;
}

int Settings::getWidth() const {
    return this->width;
}

int Settings::getHeight() const {
    return this->height;
}

const std::vector<Wall> &Settings::getWalls() const {
    return this->walls;
}

const std::vector<Checkpoint> &Settings::getCheckpoints() const {
    return this->checkpoints;
}
