//---------------------------------------------------------------------------

#ifndef GrowerH
#define GrowerH
//---------------------------------------------------------------------------

#include "GameBoard.h"

class Grower : public GameBoard {

public:
        Grower(SimplyNumbers *simplyGen, Hashtable *movesHash, int gameMode);
        void grow();
        void gridClick(int Col, int Row);
        int  getRResult();
        void restartClick();
        void moveClick();

        char* getMsg1();
        char* getMsg2();
        char* getMsg3();
        char* getMsg4();
        char* getMsg5();
        char* getMsgStatus();

protected:
        volatile int userMoveRequested;
        int resultRecieved;

private:

        bool restartRequested;
        bool takeBackRequested;
        bool exitRequested;
        bool moveRequested;

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

Grower* getXBoard(int gameMode);

#endif
 