#pragma hdrstop

#include "SimplyNumbers.h"
#include <memory.h>
#include <fstream> // Required for writing to primes.txt

SimplyNumbers::SimplyNumbers() {
    memset(hashValues, 0, sizeof(unsigned long) * 225);
    memset(simplyNumbers, 0, sizeof(unsigned long) * 225);
    simplyCounter = 0;
}
//==================================================================

//returns next simply number, unique for each move
unsigned long SimplyNumbers::getExistingHash(TMove move) {
    return hashValues[move];
}

unsigned long SimplyNumbers::getHash(TMove move) {
    unsigned long b = hashValues[move];

    // Check if we need to generate a new prime
    if (!b) {
        unsigned int next;
        if (simplyNumbers[0] != 0) {
            // Start searching from the last found prime
            next = simplyNumbers[simplyCounter];
            // Advance counter to the next slot
            ++simplyCounter;

            bool isComposite;
            do {
                ++next;
                isComposite = false;
                for(int i = 0; i < simplyCounter; ++i) {
                    // Standard prime check using modulo
                    if (next % simplyNumbers[i] == 0) {
                        isComposite = true;
                        break;
                    }
                }
            } while (isComposite);
        } else {
            // First prime initialization
            next = 2;
            simplyCounter = 0;
        }

        // Save the result
        simplyNumbers[simplyCounter] = next;
        hashValues[move] = next;
        b = next;
    }

    // Requirement: Output move to primes.txt each time function executes till end
    std::ofstream fout("primes.txt", std::ios::app);
    if (fout.is_open()) {
        // Cast TMove (unsigned char) to int to ensure decimal output
        fout << (int)move << ", ";
        fout.close();
    }

    return b;
}
