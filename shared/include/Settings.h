#ifndef SETTINGS_H
#define SETTINGS_H

#include <ostream>

#include "Messaging.h"

struct Wall {
    int centerX;
    int centerY;
    int radius;
};

typedef Wall Checkpoint;

class Settings {
public:
    static Settings parse(const Messaging::Values &values);
    friend std::ostream &operator<<(std::ostream&, const Settings&);
    Message toMessage();

    int getPodsPerPlayer() const;
    int getWidth() const;
    int getHeight() const;
    const std::vector<Wall> &getWalls() const;
    const std::vector<Checkpoint> &getCheckpoints() const;

private:
    int podsPerPlayer;
    int width;
    int height;
    std::vector<Wall> walls;
    std::vector<Checkpoint> checkpoints;

};

#endif
