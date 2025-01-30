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

        int getCell(int Col, int Row);
        void gridClick(int Col, int Row);

private:
        int userMoveRequested;
        bool swapXb, swapYb, swapWb, swapXYWb;
        int transform(int x, int y);
};

GameBoard* getXBoard(int gameMode);

#endif
 