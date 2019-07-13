#ifndef PHYSICS_H
#define PHYSICS_H

namespace Shared {
    namespace Physics {
        bool circlesOverlap(float ax, float ay, float aRadius, float bx, float by, float bRadius);
        bool containsCenter(float x, float y, float radius, float centerX, float centerY);
        float dotProduct(float ax, float ay, float bx, float by);

        float degToRad(float d);
        float radToDeg(float r);
        float vecSquareNorm(float x, float y);
        float vecNorm(float x, float y);
        float rotationTo(float x, float y);
    };
}

#endif
