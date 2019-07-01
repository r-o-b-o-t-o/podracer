#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include <string>

#include "Messaging.h"

struct PodAction {
    float rotation;
    float throttle;
};

class Action {
public:
    static Action parse(const Messaging::Values &values);
    Message toMessage();

    const PodAction &getPodAction(unsigned long long podIdx);
    const std::vector<PodAction> &getActions() const;

private:
    std::vector<PodAction> actions;
};

#endif
