#include "Turn.h"
#include "Action.h"
#include "Settings.h"
#include "Pod.h"
#include "Physics.h"

float getRotationTowardCheckpoint(Shared::Pod pod, Shared::Checkpoint checkpoint) {

    float x = checkpoint.getX() - pod.getX();
    float y = checkpoint.getY() - pod.getY();

    return -(pod.getDirection() - Shared::Physics::rotationTo(x, y))/2;
}

int main() {
    Shared::Messaging messaging;

    int playerId;
    Shared::Settings settings;
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
            pod.checkWin(checkpoints);
            a.throttle = 0.80f;
            a.rotation = getRotationTowardCheckpoint(pod, checkpoints[pod.getNextCheckpoint()]);
        }
        messaging.write(action.toMessage());
    }

    return 0;
}
