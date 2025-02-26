#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include <string>

#include "Messaging.h"

namespace Shared {
    struct PodAction {
    public:
        int pod;
        float rotation;
        float throttle;

        explicit PodAction(int pod);
    };

    class Action {
    public:
        explicit Action(int pods = 0);
        static Action parse(const Messaging::Values &values);
        Message toMessage();

        const PodAction &getPodAction(unsigned long long podIdx) const;
        PodAction &getPodAction(unsigned long long podIdx);
        const std::vector<PodAction> &getActions() const;
        std::vector<PodAction> &getActions();

    private:
        std::vector<PodAction> actions;

        static void ltrim(std::string &s);
        static void rtrim(std::string &s);
        static void trim(std::string &s);
    };
}

#endif
