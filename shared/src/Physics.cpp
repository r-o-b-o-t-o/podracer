#include <cmath>

#include "Physics.h"

namespace Shared {
    namespace Physics {
        bool circlesOverlap(float ax, float ay, float aRadius, float bx, float by, float bRadius) {
            return std::pow(bx - ax, 2.0f) + std::pow(by - ay, 2.0f) <= std::pow(aRadius + bRadius, 2.0f);
        }

        bool containsCenter(float x, float y, float radius, float centerX, float centerY) {
            return circlesOverlap(x, y, radius, centerX, centerY, 0.0f);
        }

        float dotProduct(float ax, float ay, float bx, float by) {
            return ax * bx + ay * by;
        }

        float degToRad(float d) {
            return d * static_cast<float>(M_PI) / 180.0f;
        }

        float radToDeg(float r) {
            return r * 180.0f / static_cast<float>(M_PI);
        }

        float vecSquareNorm(float x, float y) {
            return dotProduct(x, y, x, y);
        }

        float vecNorm(float x, float y) {
            return std::sqrt(vecSquareNorm(x, y));
        }

        float rotationTo(float x, float y){
            return (360 + static_cast<int>(radToDeg(atan2(y,x)))) % 360;
        }
    }
}
