#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include <string>

#include "Messaging.h"

namespace Shared {
    struct PodAction {
        float rotation;
        float throttle;
    };

    class Action {
    public:
        Action(int pods = 0);
        static Action parse(const Messaging::Values &values);
        Message toMessage();

        const PodAction &getPodAction(unsigned long long podIdx) const;
        PodAction &getPodAction(unsigned long long podIdx);
        const std::vector<PodAction> &getActions() const;

    private:
        std::vector<PodAction> actions;
    };
}

#endif
