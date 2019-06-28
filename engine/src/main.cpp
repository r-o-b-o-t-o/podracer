#include <iostream>
#include "Messaging.h"

#include <chrono>
#include <thread>

int main() {
    Messaging m;
    bool run = true;

    m.setOnMessageEvent("shutdown", [&run](Messaging &m, const std::vector<std::string> &values) {
        run = false;
        m.stop();
    });

    m.start();

    while (run) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
