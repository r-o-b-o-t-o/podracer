#include <chrono>
#include <thread>

#include "Messaging.h"
#include "Settings.h"

int main(int argc, char** argv) {
    Shared::Messaging m;
    bool run = true;

    Shared::Settings settings;
    settings.setPodsPerPlayer(argc > 1 ? std::stoi(argv[1]) : 1);
    settings.setWidth(800);
    settings.setHeight(600);
    for (int i = 0; i < 5; ++i) {
        Shared::Wall wall {};
        wall.radius = 32;
        wall.centerX = 50 + wall.radius * 2 * i;
        wall.centerY = 200;
        settings.addWall(wall);
    }
    for (int i = 0; i < 5; ++i) {
        Shared::Wall wall {};
        wall.radius = 12;
        wall.centerX = 50 + wall.radius * 2 * i;
        wall.centerY = 400;
        settings.addWall(wall);
    }
    {
        Shared::Checkpoint checkpoint {};
        checkpoint.radius = 64;
        checkpoint.centerX = 500;
        checkpoint.centerY = 400;
        settings.addCheckpoint(checkpoint);
    }

    m.setOnMessageEvent("shutdown", [&run](Shared::Messaging &m, const Shared::Messaging::Values &values) {
        run = false;
        m.stop();
    });

    m.start();
    m.output(settings.toMessage());

    while (run) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
