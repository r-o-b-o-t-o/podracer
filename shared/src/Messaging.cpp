#include <iostream>
#include <sstream>
#include <fstream>

#include "Messaging.h"

#define DEBUG_MESSAGING true

namespace Shared {
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

        if (DEBUG_MESSAGING) {
            std::ofstream f;
            f.open("messaging.log", std::ios::app);
            f << ">>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
            f << "START " << message << std::endl;
            for (const std::string &v : values) {
                f << v << std::endl;
            }
            f << "STOP " << message << std::endl;
            f << ">>>>>>>>>>>>>>>>>>>>>>>" << std::endl << std::endl;
        }

        this->writeMutex.unlock();
    }

    void Messaging::output(const Message &message) {
        this->output(message.getType(), message.getValues());
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
                this->inputs[messageName] = std::vector<std::vector<std::string>>();
            } else if (!this->currentMessage.empty()) {
                if (line.rfind("STOP ") == 0) {
                    for (auto &callback : this->onMessage) {
                        std::regex re(callback.first);
                        std::smatch match;
                        if (std::regex_search(this->currentMessage, match, re)) {
                            callback.second(*this, this->inputs[this->currentMessage], match);
                            this->currentMessage = "";
                            break;
                        }
                    }

                    if (DEBUG_MESSAGING) {
                        std::ofstream f;
                        f.open("messaging.log", std::ios::app);
                        f << "<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
                        f << "START " << this->currentMessage << std::endl;
                        for (const std::vector<std::string> &values : this->inputs[this->currentMessage]) {
                            for (const std::string &v : values) {
                                f << v << " ";
                            }
                            f << std::endl;
                        }
                        f << "STOP " << this->currentMessage << std::endl;
                        f << "<<<<<<<<<<<<<<<<<<<<<<<" << std::endl << std::endl;
                    }
                } else {
                    std::istringstream iss(line);
                    std::vector<std::string> split {
                            std::istream_iterator<std::string>(iss), {}
                    };
                    this->inputs[this->currentMessage].push_back(split);
                }
            }
        }
    }
}
