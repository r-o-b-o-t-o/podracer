#ifndef ENGINE_H
#define ENGINE_H

#include "Turn.h"
#include "Settings.h"

class Engine {
public:
    explicit Engine(int podsPerPlayer, const std::string &importExport = "");

    void importLevel(const std::string &file);
    void exportLevel(const std::string &file);

private:
    void initTrack();
    void initRandomMap();
    void readPlayers();
    void run();
    int update();
    void writeTurn();
    void readActions();

    void handleImportExport(std::string file);

    Shared::Settings settings;
    Shared::Messaging messaging;
    Shared::Turn gameState;
    int numberOfPlayers;
    bool damageEnabled;
};

#endif
