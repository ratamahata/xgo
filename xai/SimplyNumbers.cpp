//---------------------------------------------------------------------------

//TODO rename to Prime Numbers

#pragma hdrstop

#include "SimplyNumbers.h"
#include <memory.h>

SimplyNumbers::SimplyNumbers() {
    memset(hashValues, 0, sizeof(long)*225);
    simplyNumbers[0] = 0;
}
//==================================================================

//returns next simply number, unique for each move
unsigned long SimplyNumbers::getExistingHash(TMove move) {
  unsigned long b = hashValues[move];
  if (b) {
    return b;
  } else {
    return b/0;
  }
}

//returns next simply number, unique for each move
unsigned long SimplyNumbers::getHash(TMove move) {
  unsigned long b = hashValues[move];
  if (b) {
    return b;
  }
  unsigned int next;
  if (simplyNumbers[0]) {
      next = simplyNumbers[simplyCounter];
      ++simplyCounter;
      bool isComposite;
      do {
        ++next;
        isComposite = false;
        for(int i=0; i<simplyCounter; ++i)
            if (next/simplyNumbers[i]*simplyNumbers[i] == next) {
                isComposite = true;
                break;
            }
      } while (isComposite);
  } else {
      next = 2;
  }
  return hashValues[move] = simplyNumbers[simplyCounter] = next;
};


//---------------------------------------------------------------------------

#pragma package(smart_init)
