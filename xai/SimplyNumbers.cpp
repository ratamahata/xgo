#include "SimplyNumbers.h"
#include <memory.h>
#include <fstream>
#include <iostream>

SimplyNumbers::SimplyNumbers() {
    memset(hashValues, 0, sizeof(unsigned long) * 225);
    memset(simplyNumbers, 0, sizeof(unsigned long) * 225);
    simplyCounter = -1; // Initialize to -1 so first prime is at index 0

    init(); // Populate hashValues from file immediately
}

void SimplyNumbers::init() {
    std::ifstream fin(PRIMES_FILE);
    if (!fin.is_open()) return;

    int moveValue;
    char comma;
    // Read moves from file and pass them to getHash to reconstruct state
    while (fin >> moveValue) {
        getHash((TMove)moveValue);
        fin >> comma; // consume the comma
    }
    fin.close();
}

unsigned long SimplyNumbers::getExistingHash(TMove move) {
    return hashValues[move];
}

unsigned long SimplyNumbers::getHash(TMove move) {
    unsigned long b = hashValues[move];

    // If this move hasn't been assigned a prime yet
    if (b == 0) {
        unsigned int next;

        if (simplyCounter >= 0) {
            next = simplyNumbers[simplyCounter];
            bool isComposite;
            do {
                ++next;
                isComposite = false;
                for (int i = 0; i <= simplyCounter; ++i) {
                    if (next % simplyNumbers[i] == 0) {
                        isComposite = true;
                        break;
                    }
                }
            } while (isComposite);
        } else {
            next = 2; // First prime
        }

        simplyCounter++;
        simplyNumbers[simplyCounter] = next;
        hashValues[move] = next;
        b = next;

        // Persist the move to the file only if it's a new assignment
        // (Prevents duplicates during the init() loop itself)
        std::ofstream fout(PRIMES_FILE, std::ios::app);
        if (fout.is_open()) {
            fout << (int)move << ", ";
            fout.close();
        }
    }

    return b;
}
