#include <cmath>

#include "Physics.h"
#include "shared/include/Pod.h"

namespace Shared {
    const float Pod::RADIUS = 26.0f;
    const float Pod::FRICTION = 0.005f;
    const float Pod::MAX_TURN = 15.0f;
    const float Pod::MAX_THRUST = 100.0f;

    Pod::Pod(float x, float y, float vx, float vy, float direction) :
            Entity(x, y, RADIUS, 1.0f, vx, vy),
            direction(direction),
            nextCheckpoint(0),
            health(100.0f) {

    }

    float Pod::getDirection() const {
        return this->direction;
    }

    float Pod::getHealth() const {
        return this->health;
    }

    void Pod::setHealth(float health) {
        this->health = health;
    }

    void Pod::turn(float angleInDegrees) {
        this->direction += angleInDegrees;
    }

    void Pod::thrust(float power) {
        float angle = Physics::degToRad(this->direction);
        this->vx += 0.01 * std::cos(angle) * power;
        this->vy += 0.01 * std::sin(angle) * power;
    }

    void Pod::friction(float time) {
        float norm = Physics::vecNorm(this->vx, this->vy);
        float frictionX = (time * FRICTION * this->vx) / norm;
        float frictionY = (time * FRICTION * this->vy) / norm;

        if (std::abs(frictionX) >= std::abs(this->vx)) {
            frictionX = this->vx;
        }
        if (std::abs(frictionY) >= std::abs(this->vy)) {
            frictionY = this->vy;
        }

        this->vx -= frictionX;
        this->vy -= frictionY;
        if (std::isnan(this->vx)) {
            this->vx = 0.0f;
        }
        if (std::isnan(this->vy)) {
            this->vy = 0.0f;
        }
    }

    bool Pod::checkWin(const std::vector<Checkpoint> &checkpoints) {
        auto size = checkpoints.size();

        if (this->nextCheckpoint >= size) {
            return true;
        }

        while (checkpoints[this->nextCheckpoint].containsCenter(*this)) {
            this->nextCheckpoint++;
            if (this->nextCheckpoint >= size) {
                return true;
            }
        }
        return false;
    }
}
