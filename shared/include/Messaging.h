#ifndef MESSAGING_H
#define MESSAGING_H

#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <vector>

class Messaging {
public:
    typedef std::function<void(Messaging &m, const std::vector<std::string> &values)> CallbackFunction;

    Messaging();
    ~Messaging();
    void start();
    void stop();
    void output(const std::string &message, const std::vector<std::string> &values);
    void setOnMessageEvent(const std::string &message, CallbackFunction fn);

private:
    void work();

    std::thread thread;
    std::atomic<bool> run;

    std::map<std::string, std::vector<std::string>> inputs;
    std::map<std::string, CallbackFunction> onMessage;
    std::string currentMessage;

    std::mutex writeMutex;
};

#endif
