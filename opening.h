#ifndef OPENING_H
#define OPENING_H

#include "move.h"
#include <map>
#include <string>

using namespace std;

class Opening {
    private:
        void init();
    public:
        Opening();
        Move get_next();
};

#endif