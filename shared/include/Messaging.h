#ifndef MESSAGING_H
#define MESSAGING_H

#include <map>
#include <vector>
#include <functional>

#include "Message.h"

namespace Shared {
    class Messaging {
    public:
        Messaging();
        typedef std::vector<std::vector<std::string>> Values;
        typedef std::function<void(const Values &values)> CallbackFunction;

        void read(const std::string &label, const CallbackFunction &fn);
        void write(const std::string &message, const std::vector<std::string> &values);
        void write(const Message &message);

    private:
    };
}

#endif
