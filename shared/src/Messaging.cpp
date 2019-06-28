#include <iostream>

#include "../include/Messaging.h"

Messaging::Messaging() {
    this->run.store(false);
}

Messaging::~Messaging() {
    if (this->run.load()) {
        this->run.store(false);
        this->thread.join();
    }
}

void Messaging::start() {
    if (this->run.load()) {
        return;
    }

    this->run.store(true);
    this->thread = std::thread(&Messaging::work, this);
}

void Messaging::stop() {
    this->run.store(false);
}

void Messaging::output(const std::string &message, const std::vector<std::string> &values) {
    this->writeMutex.lock();

    std::cout << "START " << message << std::endl;
    for (const std::string &v : values) {
        std::cout << v << std::endl;
    }
    std::cout << "STOP " << message << std::endl;

    this->writeMutex.unlock();
}

void Messaging::setOnMessageEvent(const std::string &message, Messaging::CallbackFunction fn) {
    this->onMessage[message] = std::move(fn);
}

void Messaging::work() {
    std::string line;

    while (this->run.load()) {
        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        if (this->currentMessage.empty() && line.rfind("START ", 0) == 0) {
            std::string messageName = line.substr(6); // Rest of the line after "START "
            this->currentMessage = messageName;
            this->inputs[messageName] = std::vector<std::string>();
        } else if (!this->currentMessage.empty()) {
            if (line.rfind("STOP " + this->currentMessage) == 0) {
                if (this->onMessage.find(this->currentMessage) != this->onMessage.end()) {
                    this->onMessage[this->currentMessage](*this, this->inputs[this->currentMessage]);
                }
                this->currentMessage = "";
            } else {
                this->inputs[this->currentMessage].push_back(line);
            }
        }
    }
}
