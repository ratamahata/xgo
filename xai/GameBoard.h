//---------------------------------------------------------------------------

#ifndef GameBoardH
#define GameBoardH
//---------------------------------------------------------------------------

#include "Builder.h"

//can flip board
class GameBoard : public Builder {

public:
        bool *swapX, *swapY, *swapW, *swapXYW;

        GameBoard(SimplyNumbers *simplyGen, Hashtable *movesHash, int gameMode);

        bool put(TMove N);//can flip board
        int move();
        void build();

private:
        bool swapXb, swapYb, swapWb, swapXYWb;


};

GameBoard* createXBoard(int gameMode);

#endif
 