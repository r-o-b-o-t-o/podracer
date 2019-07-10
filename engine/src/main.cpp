#include "Engine.h"

int main(int argc, char** argv) {
    Engine engine(argc > 1 ? std::stoi(argv[1]) : 1);
    return 0;
}
