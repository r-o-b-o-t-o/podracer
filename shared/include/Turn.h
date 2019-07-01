#ifndef TURN_H
#define TURN_H

#include "Messaging.h"

struct State {
    int x;
    int y;
    float vx;
    float vy;
    float direction;
    float health;
};

class Turn {
public:
    static Turn parse(const Messaging::Values &values);
    Message toMessage();

    unsigned long long numberOfPlayers() const;
    unsigned long long numberOfPods(unsigned long long playerIdx) const;
    const std::vector<std::vector<State>> &getPlayerStates() const;
    const std::vector<State> &getPlayerState(unsigned long long playerIdx) const;
    const State &getPodState(unsigned long long playerIdx, unsigned long long podIdx) const;

private:
    std::vector<std::vector<State>> playerStates;
};

#endif
