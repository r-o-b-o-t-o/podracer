#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>

#include "Messaging.h"

#define DEBUG_MESSAGING false

namespace Shared {
    Messaging::Messaging() {
        if (DEBUG_MESSAGING) {
            std::ofstream f;
            f.open("messaging.log", std::ios::out);
        }
    }

    void Messaging::write(const std::string &message, const std::vector<std::string> &values) {
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
    }

    void Messaging::write(const Message &message) {
        this->write(message.getType(), message.getValues());
    }

    void Messaging::read(const std::string &label, const CallbackFunction &fn) {
        std::string line;
        bool started = false;
        Values inputs;

        while (true) {
            std::getline(std::cin, line);

            if (line.empty()) {
                continue;
            }

            if (!started) {
                if (line == "START " + label) {
                    started = true;
                }
            } else {
                if (line.rfind("STOP ") == 0) {
                    fn(inputs);

                    if (DEBUG_MESSAGING) {
                        std::ofstream f;
                        f.open("messaging.log", std::ios::app);
                        f << "<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
                        f << "START " << label << std::endl;
                        for (const std::vector<std::string> &values : inputs) {
                            for (const std::string &v : values) {
                                f << v << " ";
                            }
                            f << std::endl;
                        }
                        f << "STOP " << label << std::endl;
                        f << "<<<<<<<<<<<<<<<<<<<<<<<" << std::endl << std::endl;
                    }

                    break;
                } else {
                    std::istringstream iss(line);
                    std::vector<std::string> split {
                        std::istream_iterator<std::string>(iss), {}
                    };
                    inputs.push_back(split);
                }
            }
        }
    }
}
