#ifndef TURN_H
#define TURN_H

#include "shared/include/Pod.h"
#include "Messaging.h"

namespace Shared {
    class Turn {
    public:
        Turn();
        static Turn parse(const Messaging::Values &values);
        void update(const Messaging::Values &values);
        Message toMessage(int player);

        unsigned long long numberOfPlayers() const;
        unsigned long long numberOfPods(unsigned long long playerIdx) const;
        const std::vector<std::vector<Pod>> &getPlayerStates() const;
        std::vector<std::vector<Pod>> &getPlayerStates();
        const std::vector<Pod> &getPlayerState(unsigned long long playerIdx) const;
        std::vector<Pod> &getPlayerState(unsigned long long playerIdx);
        const Pod &getPodState(unsigned long long playerIdx, unsigned long long podIdx) const;
        Pod &getPodState(unsigned long long playerIdx, unsigned long long podIdx);
        unsigned long getTurn() const;
        void nextTurn();

    private:
        unsigned long turnIdx;
        std::vector<std::vector<Pod>> playerStates;
    };
}

#endif
