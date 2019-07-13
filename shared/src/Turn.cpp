#include <cmath>
#include <sstream>

#include "Turn.h"

namespace Shared {
    Turn::Turn() :
            turnIdx(1) {

    }

    Turn Turn::parse(const Messaging::Values &values) {
        Turn turn {};
        for (auto it = values.begin(); it < values.end(); ++it) {
            auto line = *it;

            int playerIdx = std::stoi(line[0]) - 1;
            float x = std::stoi(line[2]);
            float y = std::stoi(line[3]);
            float vx = std::stof(line[4]);
            float vy = std::stof(line[5]);
            float direction = std::stof(line[6]);
            float health = std::stof(line[7]);
            Pod pod(x, y, vx, vy, direction);
            pod.setHealth(health);

            if (turn.playerStates.size() <= playerIdx) {
                turn.playerStates.emplace_back();
            }
            turn.playerStates[playerIdx].push_back(pod);
        }

        return turn;
    }

    void Turn::update(const Messaging::Values &values) {
        for (auto it = values.begin(); it < values.end(); ++it) {
            auto line = *it;

            int playerIdx = std::stoi(line[0]) - 1;
            int podIdx = std::stoi(line[1]) - 1;
            float x = std::stoi(line[2]);
            float y = std::stoi(line[3]);
            float vx = std::stof(line[4]);
            float vy = std::stof(line[5]);
            float direction = std::stof(line[6]);
            float health = std::stof(line[7]);

            if (this->playerStates.size() <= playerIdx) {
                this->playerStates.emplace_back();
            }
            if (this->playerStates[playerIdx].size() <= podIdx) {
                Pod pod(x, y, vx, vy, direction);
                pod.setHealth(health);
                this->playerStates[playerIdx].push_back(pod);
            }

            this->playerStates[playerIdx][podIdx].setX(x);
            this->playerStates[playerIdx][podIdx].setY(y);
            this->playerStates[playerIdx][podIdx].setVx(vx);
            this->playerStates[playerIdx][podIdx].setVy(vy);
            this->playerStates[playerIdx][podIdx].setDirection(direction);
            this->playerStates[playerIdx][podIdx].setHealth(health);
        }
    }

    Message Turn::toMessage(int player) {
        Message m("turn " + std::to_string(this->turnIdx) + " " + std::to_string(player));

        for (int playerIdx = 0; playerIdx < this->playerStates.size(); ++playerIdx) {
            const std::vector<Pod> &states = this->playerStates[playerIdx];
            for (int podIdx = 0; podIdx < states.size(); ++podIdx) {
                const Pod &pod = states[podIdx];
                std::stringstream stream;

                stream << playerIdx + 1 << " ";
                stream << podIdx + 1 << " ";
                stream << static_cast<int>(std::round(pod.getX())) << " ";
                stream << static_cast<int>(std::round(pod.getY())) << " ";
                stream << pod.getVx() << " ";
                stream << pod.getVy() << " ";
                stream << pod.getDirection() << " ";
                stream << pod.getHealth();
                m.addValue(stream.str());
            }
        }

        return m;
    }

    const std::vector<std::vector<Pod>> &Turn::getPlayerStates() const {
        return this->playerStates;
    }

    std::vector<std::vector<Pod>> &Turn::getPlayerStates() {
        return this->playerStates;
    }

    const std::vector<Pod> &Turn::getPlayerState(unsigned long long playerIdx) const {
        return this->playerStates.at(playerIdx);
    }

    std::vector<Pod> &Turn::getPlayerState(unsigned long long playerIdx) {
        return this->playerStates.at(playerIdx);
    }

    const Pod &Turn::getPodState(unsigned long long playerIdx, unsigned long long podIdx) const {
        return this->playerStates.at(playerIdx).at(podIdx);
    }

    Pod &Turn::getPodState(unsigned long long playerIdx, unsigned long long podIdx) {
        return this->playerStates.at(playerIdx).at(podIdx);
    }

    unsigned long long Turn::numberOfPlayers() const {
        return this->playerStates.size();
    }

    unsigned long long Turn::numberOfPods(unsigned long long playerIdx) const {
        return this->playerStates.at(playerIdx).size();
    }

    unsigned long Turn::getTurn() const {
        return this->turnIdx;
    }

    void Turn::nextTurn() {
        this->turnIdx++;
    }
}
