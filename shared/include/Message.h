#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>

class Message {
public:
    Message(const std::string &type);
    void addValue(const std::string &v);

    const std::string &getType() const;
    const std::vector<std::string> &getValues() const;

private:
    std::string type;
    std::vector<std::string> values;

};

#endif
