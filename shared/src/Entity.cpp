#include <cmath>

#include "Entity.h"
#include "Physics.h"

namespace Shared {
    namespace Physics {
        Entity::Entity(float x, float y, float radius, float m, float vx, float vy) :
                x(x),
                y(y),
                radius(radius),
                m(m),
                vx(vx),
                vy(vy) {

        }

        float Entity::getX() const {
            return this->x;
        }

        float Entity::getY() const {
            return this->y;
        }

        float Entity::getVx() const {
            return this->vx;
        }

        float Entity::getVy() const {
            return this->vy;
        }

        void Entity::setX(float x) {
            this->x = x;
        }

        void Entity::setY(float y) {
            this->y = y;
        }

        void Entity::setVx(float vx) {
            this->vx = vx;
        }

        void Entity::setVy(float vy) {
            this->vy = vy;
        }

        float Entity::getRadius() const {
            return this->radius;
        }

        float Entity::getMass() const {
            return m;
        }

        void Entity::setMass(float m) {
            this->m = m;
        }

        float Entity::collisionTime(const Entity &other) const {
            float a = vecSquareNorm(this->vx, this->vy) + vecSquareNorm(other.vx, other.vy)
                        - 2 * dotProduct(this->vx, this->vy, other.vx, other.vy);

            float b = 2 * (dotProduct(this->x, this->y, this->vx, this->vy) +
                           dotProduct(other.x, other.y, other.vx, other.vy) -
                           dotProduct(this->x, this->y, other.vx, other.vy) -
                           dotProduct(other.x, other.y, this->vx, this->vy));

            float c = vecSquareNorm(this->x, this->y) + vecSquareNorm(other.x, other.y)
                        - 2 * dotProduct(this->x, this->y, other.x, other.y)
                        - std::pow(this->radius + other.radius, 2.0f);

            float delta = std::pow(b, 2.0f) - 4.0f * a * c;
            if (delta < 0.0f) {
                return -1;
            }
            if (delta > 0.0f && a != 0.0f) {
                delta = std::sqrt(delta);
                float t1 = (-b - delta) / (2 * a);
                float t2 = (-b + delta) / (2 * a);
                if (t1 >= 0) {
                    return t1;
                }
                return t2;
            }
            return -1.0f;
        }

        void Entity::impactRedirection(Entity &other) {
            float nx = other.x - this->x;
            float ny = other.y - this->y;
            float unorm = Physics::vecNorm(nx, ny);
            float unx = nx / unorm;
            float uny = ny / unorm;
            float utx = -uny;
            float uty = unx;
            float v1n = dotProduct(unx, uny, this->vx, this->vy);
            float v2n = dotProduct(unx, uny, other.vx, other.vy);
            float v1t = dotProduct(utx, uty, this->vx, this->vy);
            float v2t = dotProduct(utx, uty, other.vx, other.vy);
            float v1nf = (v1n * (this->m - other.m) + 2 * other.m * v2n) / (this->m + other.m);
            float v2nf = (v2n * (other.m - this->m) + 2 * this->m * v1n) / (this->m + other.m);

            this->vx = (v1nf * unx + v1t * utx);
            this->vy = (v1nf * uny + v1t * uty);
            other.vx = (v2nf * unx + v2t * utx);
            other.vy = (v2nf * uny + v2t * uty);
        }

        bool Entity::intersects(const Entity &other) const {
            return circlesOverlap(this->x, this->y, this->radius, other.x, other.y, other.radius);
        }

        bool Entity::containsCenter(const Entity &other) const {
            return Physics::containsCenter(this->x, this->y, this->radius, other.x, other.y);
        }

        void Entity::updatePosition(float time) {
            this->x += this->vx * time;
            this->y += this->vy * time;
        }
    }
}
