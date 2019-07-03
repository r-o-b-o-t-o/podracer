#include "Action.h"

namespace Shared {
    Action Action::parse(const Messaging::Values &values) {
        Action a {};

        std::vector<std::string> line = values[0];
        for (int i = 0; i < line.size(); i += 2) {
            std::string rotation = line[i];
            std::string throttle = line[i + 1];
            if (throttle[throttle.length() - 1] == ';') {
                throttle = throttle.substr(0, throttle.length() - 1);
            }
            PodAction action {};
            action.rotation = static_cast<float>(std::stod(rotation));
            action.throttle = static_cast<float>(std::stod(throttle));
            a.actions.push_back(action);
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

    const PodAction &Action::getPodAction(unsigned long long podIdx) {
        return this->actions.at(podIdx);
    }

    const std::vector<PodAction> &Action::getActions() const {
        return this->actions;
    }
}
