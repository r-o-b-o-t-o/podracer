#ifndef SHARED_POD_H
#define SHARED_POD_H

#include "Entity.h"
#include "Settings.h"

namespace Shared {
    class Pod : public Shared::Physics::Entity {
    public:
        Pod(float x, float y, float vx, float vy, float direction);

        float getDirection() const;
        void setDirection(float direction);
        float getHealth() const;
        void setHealth(float health);
        int getNextCheckpoint() const;
        void setNextCheckpoint(int nextCheckpoint);

        void turn(float angleInDegrees);
        void thrust(float power);
        void friction(float time);
        bool checkWin(const std::vector<Checkpoint> &checkpoints);

        static const float RADIUS;
        static const float FRICTION;
        static const float MAX_TURN;
        static const float MAX_THRUST;

    private:
        float direction;
        float health;
        int nextCheckpoint;
    };
}

#endif
