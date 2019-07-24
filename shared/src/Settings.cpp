#include <sstream>

#include "Settings.h"

namespace Shared {
    Wall::Wall(float x, float y, float radius) :
            Entity(x, y, radius) {

    }

    Wall::Wall(const Shared::Physics::Entity &e) : Entity(e) {}

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
                    walls.emplace_back(
                        std::stoi(line[0]),
                        std::stoi(line[1]),
                        std::stoi(line[2])
                    );
                }
                settings.walls = walls;
            } else if (line[0] == "CHECKPOINTS") {
                int n = std::stoi(line[1]);
                std::vector<Checkpoint> checkpoints;

                for (int i = 0; i < n; ++i) {
                    ++it;
                    line = *it;
                    checkpoints.emplace_back(
                        std::stoi(line[0]),
                        std::stoi(line[1]),
                        std::stoi(line[2])
                    );
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
            m.addValue(std::to_string(static_cast<int>(wall.getX())) + " " +
                       std::to_string(static_cast<int>(wall.getY())) + " " +
                       std::to_string(static_cast<int>(wall.getRadius())));
        }
        m.addValue("CHECKPOINTS " + std::to_string(this->checkpoints.size()));
        for (const Checkpoint &checkpoint : this->checkpoints) {
            m.addValue(std::to_string(static_cast<int>(checkpoint.getX())) + " " +
                       std::to_string(static_cast<int>(checkpoint.getY())) + " " +
                       std::to_string(static_cast<int>(checkpoint.getRadius())));
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
            stream << "        Wall { x: " << wall.getX() << ", y: " << wall.getY() << ", radius: " << wall.getRadius()
                   << " }";
            if (it + 1 != settings.walls.end()) {
                stream << ",";
            }
            stream << std::endl;
        }
        stream << "    ]," << std::endl;
        stream << "    Checkpoints: [" << std::endl;
        for (auto it = settings.checkpoints.begin(); it != settings.checkpoints.end(); ++it) {
            const Checkpoint &checkpoint = *it;
            stream << "        Checkpoint { x: " << checkpoint.getX() << ", y: " << checkpoint.getY() << ", radius: "
                   << checkpoint.getRadius() << " }";
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

    std::vector<Wall> &Settings::getWalls() {
        return this->walls;
    }

    const std::vector<Checkpoint> &Settings::getCheckpoints() const {
        return this->checkpoints;
    }

    std::vector<Checkpoint> &Settings::getCheckpoints() {
        return this->checkpoints;
    }

    void Settings::setPodsPerPlayer(int podsPerPlayer) {
        this->podsPerPlayer = podsPerPlayer;
    }

    void Settings::setWidth(int width) {
        this->width = width;
    }

    void Settings::setHeight(int height) {
        this->height = height;
    }

    void Settings::addWall(const Wall &wall) {
        this->walls.push_back(wall);
    }

    void Settings::addCheckpoint(const Checkpoint &checkpoint) {
        this->checkpoints.push_back(checkpoint);
    }
}
