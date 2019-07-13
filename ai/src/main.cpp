#include <cmath>

#include "Turn.h"
#include "Action.h"
#include "Settings.h"
#include "Pod.h"
#include "Physics.h"

float MAX_THRUST = 100.0f;
float EXPO = 1.2f;
float MAX_SPEED = 1000.0f;
float MIN_SPEED = 1.0f;

float getRotationTowardCheckpoint(Shared::Pod pod, Shared::Checkpoint checkpoint) {

    float x = checkpoint.getX() - pod.getX();
    float y = checkpoint.getY() - pod.getY();

    return -(pod.getDirection() - Shared::Physics::rotationTo(x, y))/2;
}

float getThrustTowardCheckpoint(Shared::Pod pod, Shared::Checkpoint checkpoint) {

    float x = checkpoint.getX() - pod.getX();
    float y = checkpoint.getY() - pod.getY();

    float normVec = Shared::Physics::vecNorm(x, y);
    x /= normVec;
    y /= normVec;

    float xSpeed = pod.getVx();
    float ySpeed = pod.getVy();

    float normSpeed = Shared::Physics::vecNorm(xSpeed, ySpeed);

    xSpeed /= normSpeed;
    ySpeed /= normSpeed;

    if (Shared::Physics::dotProduct(x, y, xSpeed, ySpeed) > 0.5){
        if (normSpeed > MAX_SPEED)
            return 0;
        if (normSpeed < MIN_SPEED)
            return MAX_THRUST;
    }

    float thrust = std::pow(normVec, EXPO);

    if (thrust > MAX_THRUST)
        return MAX_THRUST;

    return thrust;
}

void parseArgs(int argc, char** argv) {
    if (argc > 1)
        MAX_THRUST = std::stof(argv[1]);
    if (argc > 2)
        EXPO = std::stof(argv[2]);
    if (argc > 3)
        MIN_SPEED = std::stof(argv[3]);
    if (argc > 4)
        MAX_SPEED = std::stof(argv[4]);
}

int main(int argc, char** argv) {

    int playerId;
    Shared::Settings settings;
    Shared::Messaging messaging;

    parseArgs(argc, argv);

    messaging.read("player", [&playerId](const Shared::Messaging::Values &values) {
        playerId = std::stoi(values[0][0]);
    });
    messaging.read("settings", [&settings](const Shared::Messaging::Values &values) {
        settings = Shared::Settings::parse(values);
    });

    std::vector<Shared::Checkpoint> checkpoints = settings.getCheckpoints();


    Shared::Turn turn = Shared::Turn();
    while (true) {
        messaging.read("turn", [&](const Shared::Messaging::Values &values) {
            turn.update(values);
        });

        Shared::Action action(settings.getPodsPerPlayer());
        for (auto &a : action.getActions()) {
            Shared::Pod &pod = turn.getPodState(playerId-1, a.pod);
            if (pod.getHealth() <= 0)
                continue;
            pod.checkWin(checkpoints);
            a.throttle = getThrustTowardCheckpoint(pod, checkpoints[pod.getNextCheckpoint()]);
            a.rotation = getRotationTowardCheckpoint(pod, checkpoints[pod.getNextCheckpoint()]);
        }
        messaging.write(action.toMessage());
    }

    return 0;
}
