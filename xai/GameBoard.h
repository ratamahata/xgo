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
        void grow();

        int getCell(int Col, int Row);
        void gridClick(int Col, int Row);

private:
        bool swapXb, swapYb, swapWb, swapXYWb;
        int transform(int x, int y);

        bool restartRequested;
        bool takeBackRequested;
        bool exitRequested;
        bool moveRequested;
        int resultRecieved;
        int userMoveRequested;
        int movesCount;

        unsigned char dkl[fsize*fsize];

        char msg1[200];
        char msg2[200];
        char msg3[200];
        char msg4[200];
        char msg5[200];
        short int xRating;

        char msgStatus[200];

};

GameBoard* getXBoard(int gameMode);

#endif
 