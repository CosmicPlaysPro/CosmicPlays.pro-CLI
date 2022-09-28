#pragma once
#include "InputParser.h"

class ParamManager {
private:
    InputParser input;
public:
    ParamManager(int& argc, char** argv) {
        input = InputParser(argc, argv);
    }

    bool parseParams() {

    }
};
