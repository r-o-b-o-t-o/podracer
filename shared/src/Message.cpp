#include "Message.h"

Message::Message(const std::string &type) :
    type(type) {

}

void Message::addValue(const std::string &v) {
    this->values.push_back(v);
}

const std::string &Message::getType() const {
    return this->type;
}

const std::vector<std::string> &Message::getValues() const {
    return this->values;
}
