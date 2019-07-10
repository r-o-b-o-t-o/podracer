#ifndef SETTINGS_H
#define SETTINGS_H

#include <ostream>

#include "Entity.h"
#include "Messaging.h"

namespace Shared {
    class Wall : public Physics::Entity {
    public:
        Wall(float x, float y, float radius);
    };

    typedef Wall Checkpoint;

    class Settings {
    public:
        static Settings parse(const Messaging::Values &values);
        friend std::ostream &operator<<(std::ostream &, const Settings &);
        Message toMessage();

        int getPodsPerPlayer() const;
        int getWidth() const;
        int getHeight() const;
        const std::vector<Wall> &getWalls() const;
        std::vector<Wall> &getWalls();
        const std::vector<Checkpoint> &getCheckpoints() const;
        std::vector<Checkpoint> &getCheckpoints();

        void setPodsPerPlayer(int podsPerPlayer);
        void setWidth(int width);
        void setHeight(int height);
        void addWall(const Wall &wall);
        void addCheckpoint(const Checkpoint &checkpoint);

    private:
        int podsPerPlayer;
        int width;
        int height;
        std::vector<Wall> walls;
        std::vector<Checkpoint> checkpoints;

    };
}

#endif
