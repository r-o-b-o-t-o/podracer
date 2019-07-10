#include "Action.h"

namespace Shared {
    Action::Action(int pods) {
        for (int i = 0; i < pods; ++i) {
            this->actions.emplace_back();
        }
    }

    Action Action::parse(const Messaging::Values &values) {
        Action a {};
        bool empty = values[0][0] == "NOACTION";

        if (!empty) {
            int podIdx = 0;
            std::vector<std::string> parts = values[0];
            std::string line;
            for (auto &p : parts) {
                line += p + " ";
            }
            trim(line);

            std::vector<std::string> actions;
            std::string delimiter = ";";
            size_t last = 0;
            size_t next = 0;
            while ((next = line.find(delimiter, last)) != std::string::npos) {
                std::string value = line.substr(last, next-last);
                trim(value);
                actions.push_back(value);
                last = next + 1;
            }
            std::string lastItem = line.substr(last);
            if (!lastItem.empty()) {
                trim(lastItem);
                actions.push_back(lastItem);
            }

            for (auto &s : actions) {
                PodAction podAction {};
                podAction.pod = podIdx;
                podAction.rotation = std::stof(s.substr(0, s.find(' ')));
                podAction.throttle = std::stof(s.substr(s.find(' ') + 1));
                a.actions.push_back(podAction);
                ++podIdx;
            }
        }

        return a;
    }

    Message Action::toMessage() {
        Message m("action");
        std::string line;
        for (int i = 0; i < this->actions.size(); ++i) {
            const PodAction &action = this->actions[i];
            line += std::to_string(action.rotation) + " ";
            line += std::to_string(action.throttle);
            if (i != this->actions.size() - 1) {
                line += "; ";
            }
        }
        m.addValue(line);

        return m;
    }

    const PodAction &Action::getPodAction(unsigned long long podIdx) const {
        return this->actions.at(podIdx);
    }

    PodAction &Action::getPodAction(unsigned long long podIdx) {
        return this->actions.at(podIdx);
    }

    const std::vector<PodAction> &Action::getActions() const {
        return this->actions;
    }

    void Action::ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    void Action::rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    void Action::trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }
}
