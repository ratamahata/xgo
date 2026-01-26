#ifndef simplynumbersH
#define simplynumbersH

#include "TNode.h"
#include <string>

const std::string PRIMES_FILE = "primes.txt";

class SimplyNumbers {
public:
    unsigned long simplyNumbers[225];
    unsigned long hashValues[225];
    int simplyCounter;

    unsigned long getExistingHash(TMove move);
    unsigned long getHash(TMove move);
    void init(); // Load existing state from file

    SimplyNumbers();
};

#endif
