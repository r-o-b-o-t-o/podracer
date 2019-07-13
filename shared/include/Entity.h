#ifndef PHYSICS_ENTITY_H
#define PHYSICS_ENTITY_H

namespace Shared {
    namespace Physics {
        class Entity {
        public:
            Entity(float x, float y, float radius, float m = 1'000'000.0f, float vx = 0.0f, float vy = 0.0f);

            float getX() const;
            float getY() const;
            float getVx() const;
            float getVy() const;
            void setX(float x);
            void setY(float y);
            void setVx(float vx);
            void setVy(float vy);
            float getRadius() const;
            float getMass() const;
            void setMass(float m);

            bool intersects(const Entity &other) const;
            bool containsCenter(const Entity &other) const;
            float collisionTime(const Entity &other) const;
            void impactRedirection(Entity &other);
            void updatePosition(float time);

        protected:
            float x;
            float y;
            float vx;
            float vy;
            float radius;
            float m;
        };
    }
}

#endif
