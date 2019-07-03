#include <sstream>

#include "Turn.h"

namespace Shared {
    Turn Turn::parse(const Messaging::Values &values) {
        Turn turn {};
        for (auto it = values.begin(); it < values.end(); ++it) {
            auto line = *it;

            int playerIdx = std::stoi(line[0]) - 1;
            State state {};

            state.x = std::stoi(line[2]);
            state.y = std::stoi(line[3]);
            state.vx = std::stof(line[4]);
            state.vy = std::stof(line[5]);
            state.direction = std::stof(line[6]);
            state.health = std::stof(line[7]);

            if (turn.playerStates.size() <= playerIdx) {
                turn.playerStates.emplace_back();
            }
            turn.playerStates[playerIdx].push_back(state);
        }

        return turn;
    }

    Message Turn::toMessage() {
        Message m("turn");

        for (int playerIdx = 0; playerIdx < this->playerStates.size(); ++playerIdx) {
            const std::vector<State> &states = this->playerStates[playerIdx];
            for (int podIdx = 0; podIdx < states.size(); ++podIdx) {
                const State &state = states[podIdx];
                std::stringstream stream;

                stream << playerIdx + 1 << " ";
                stream << podIdx + 1 << " ";
                stream << state.x << " ";
                stream << state.y << " ";
                stream << state.vx << " ";
                stream << state.vy << " ";
                stream << state.direction << " ";
                stream << state.health;
                m.addValue(stream.str());
            }
        }

        return m;
    }

    const std::vector<std::vector<State>> &Turn::getPlayerStates() const {
        return this->playerStates;
    }

    const std::vector<State> &Turn::getPlayerState(unsigned long long playerIdx) const {
        return this->playerStates.at(playerIdx);
    }

    const State &Turn::getPodState(unsigned long long playerIdx, unsigned long long podIdx) const {
        return this->playerStates.at(playerIdx).at(podIdx);
    }

    unsigned long long Turn::numberOfPlayers() const {
        return this->playerStates.size();
    }

    unsigned long long Turn::numberOfPods(unsigned long long playerIdx) const {
        return this->playerStates.at(playerIdx).size();
    }
}
