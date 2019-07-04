#include "Turn.h"
#include "Action.h"
#include "Settings.h"

int main() {
    Shared::Messaging messaging;

    int playerId;
    Shared::Settings settings;
    messaging.read("player", [&playerId](const Shared::Messaging::Values &values, const std::smatch &match) {
        playerId = std::stoi(values[0][0]);
    });
    messaging.read("settings", [&settings](const Shared::Messaging::Values &values, const std::smatch &match) {
        settings = Shared::Settings::parse(values);
    });

    while (true) {
        Shared::Turn turn;
        messaging.read("turn", [&](const Shared::Messaging::Values &values, const std::smatch &match) {
            turn = Shared::Turn::parse(values);
        });

        Shared::Action action(settings.getPodsPerPlayer());
        messaging.write(action.toMessage());
    }

    return 0;
}
