//---------------------------------------------------------------------------

#ifndef simplynumbersH
#define simplynumbersH

#include "TNode.h"
//---------------------------------------------------------------------------

//TODO rename to Prime Numbers

class SimplyNumbers {
public:
  unsigned long simplyNumbers[225]; //ordinal->simplyNumber
  unsigned long hashValues[225]; // move->simplyNumber
  int simplyCounter;
  unsigned long getExistingHash(TMove move);
  unsigned long getHash(TMove move);

  SimplyNumbers();
};

#endif
