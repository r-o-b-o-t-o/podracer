#ifndef ENGINE_H
#define ENGINE_H

#include "Turn.h"
#include "Settings.h"

class Engine {
public:
    explicit Engine(int numberOfPlayers);

private:
    void init();
    void readPlayers();
    void run();
    int update();
    void writeTurn();
    void readActions();

    Shared::Settings settings;
    Shared::Messaging messaging;
    Shared::Turn gameState;
    int numberOfPlayers;
    bool damageEnabled;
};

#endif
