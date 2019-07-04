#ifndef TURN_H
#define TURN_H

#include "Messaging.h"

namespace Shared {
    struct State {
        float x;
        float y;
        float vx;
        float vy;
        float direction;
        float health;
    };

    class Turn {
    public:
        Turn();
        static Turn parse(const Messaging::Values &values);
        Message toMessage(int player);

        unsigned long long numberOfPlayers() const;
        unsigned long long numberOfPods(unsigned long long playerIdx) const;
        const std::vector<std::vector<State>> &getPlayerStates() const;
        std::vector<std::vector<State>> &getPlayerStates();
        const std::vector<State> &getPlayerState(unsigned long long playerIdx) const;
        std::vector<State> &getPlayerState(unsigned long long playerIdx);
        const State &getPodState(unsigned long long playerIdx, unsigned long long podIdx) const;
        unsigned long getTurn() const;
        void nextTurn();

    private:
        unsigned long turnIdx;
        std::vector<std::vector<State>> playerStates;
    };
}

#endif
