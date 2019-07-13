#include "Engine.h"

int main(int argc, char** argv) {
    int podsPerPlayer = argc > 1 ? std::stoi(argv[1]) : 1;
    std::string importExport = argc > 2 ? argv[2] : "";

    Engine engine(podsPerPlayer, importExport);
    return 0;
}
