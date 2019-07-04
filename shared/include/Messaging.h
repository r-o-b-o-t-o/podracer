#ifndef MESSAGING_H
#define MESSAGING_H

#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <vector>
#include <regex>

#include "Message.h"

namespace Shared {
    class Messaging {
    public:
        typedef std::vector<std::vector<std::string>> Values;
        typedef std::function<void(Messaging &m, const Values &values, const std::smatch &match)> CallbackFunction;

        Messaging();
        ~Messaging();
        void start();
        void stop();
        void output(const std::string &message, const std::vector<std::string> &values);
        void output(const Message &message);
        void setOnMessageEvent(const std::string &message, CallbackFunction fn);

    private:
        void work();

        std::thread thread;
        std::atomic<bool> run;

        std::map<std::string, Values> inputs;
        std::map<std::string, CallbackFunction> onMessage;
        std::string currentMessage;

        std::mutex writeMutex;
    };
}

#endif
