//---------------------------------------------------------------------------


#pragma hdrstop

#include "Grower.h"
#include <cstring>
#include <thread>
#include <chrono>

//---------------------------------------------------------------------------

#pragma package(smart_init)

Grower::Grower(SimplyNumbers *simplyGen, Hashtable *movesHash,
                  int gameMode) :  GameBoard(simplyGen, movesHash, gameMode) {

    bool isCreated;
    TNode *node = movesHash->getOrCreate(1, 1, 0, isCreated);

    forward(112, node);
    expand();
};



#define MAX_CHILDS_WIZARD  40000000

#define MAX_CHILD_PER_MOVE_ZONE0  6000000
#define MAX_CHILD_PER_MOVE_ZONE1  1000000

#define MAX_CHILD_DECREASE_FACTOR 6
#define MAX_CHILD_DECREASE_SINCE 16000000
#define MAX_CHILD_DECREASE_TILL 32000000

#define ZONE01_RATING 5700
#define ZONE12_RATING 10000


void Grower::grow() {
  static int count;

  int wizardMode = 1000;
  short int flowRating = 0;

//   SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL)) 

  int childs0 = 0;
  int playMode = 1;//0= Human vs Human, 1= Comp vs Human, 2= Comp vs Comp, 3= Debuts calc
   
//  unsigned long beginTime = GetTickCount();
  while (!exitRequested) {

        //********* STEP 1   initialize ***************

        if (playMode < 2) { //Human vs Human or Human vs Comp
              wizardMode = 0;
        } else if (wizardMode == 0 && playMode >= 2) { // Comp vs Comp or Debuts calculation
            wizardMode = 1000;
        }

        bool changed = true;

        TNode *firstNode = getFirstNode();

        TRating currRating = lastMove()->rating;
        TRating currXRating = count%2
                        ? currRating
                        : -currRating;

        int delta = currXRating - firstNode->rating;
        if (delta < 0) {
                delta = -delta;
        }
        bool medRating = (delta < 500);
        unsigned int totalChilds = firstNode->totalChilds;

        bool mediumicPlay = medRating;
/*              && (count <  4
                || (count <  5 && totalChilds >  500000)
                || (count <  6 && totalChilds >  600000)
                || (count <  7 && totalChilds >  700000)
                || (count <  8 && totalChilds >  800000)
                || (count <  9 && totalChilds > 1200000)
                || (count < 11 && totalChilds > 1400000)
                || (count < 13 && totalChilds > 2000000)
                || (count < 15 && totalChilds > 3000000)
                || (count < 17 && totalChilds > 4000000)
                || (count < 18 && totalChilds > 7000000)
                || (count < 19 && totalChilds >18000000)
                || (count < 20 && totalChilds >20000000)
                || (count < 21 && totalChilds >23000000)
                );*/

        int childPerMove = 175000;//CSpinEditChilds->Value*1000;
        if (wizardMode && childs0 && childs0 >= childPerMove && childs0 < childPerMove*2) {
                childPerMove = childs0 + 50000;
        }

        double mcdf = totalChilds > MAX_CHILD_DECREASE_SINCE
                ? totalChilds > MAX_CHILD_DECREASE_TILL
                        ? MAX_CHILD_DECREASE_FACTOR
                        : MAX_CHILD_DECREASE_FACTOR
                                * ((totalChilds - MAX_CHILD_DECREASE_SINCE)/1000)
                                / (double)((MAX_CHILD_DECREASE_TILL - MAX_CHILD_DECREASE_SINCE)/1000)
                : 1;
        if (mcdf < 1) mcdf=1;

        int maxChilds = currRating < ZONE01_RATING && currRating > -ZONE01_RATING
                ? MAX_CHILD_PER_MOVE_ZONE0 / mcdf
                : currRating < ZONE12_RATING && currRating > -ZONE12_RATING
                        ? MAX_CHILD_PER_MOVE_ZONE1 / mcdf
                        :childPerMove + 1;
        if (maxChilds < childPerMove) {
                maxChilds = childPerMove;
        }

        //********* STEP 2   process requested actions ***************
        if (restartRequested) {
            restartRequested = false;
            restart();
        } else if (userMoveRequested != 255) {
                int i;
                bool res = put(userMoveRequested);
                if (res) {
                        int totl = lastMove()->totalChilds;
                        resultRecieved = lastMove()->rating;
                        if (res < 32600) {
                            moveRequested = true;
                        }
                }
                userMoveRequested = 255;

        } else if (moveRequested &&
                (wizardMode || lastMove()->totalChilds >= childPerMove
                        || lastMove()->rating < -20000
                        || lastMove()->rating > 20000
                        || lastMove()->totalDirectChilds == 1)) {

          moveRequested = false;
          flowRating = currRating;
          resultRecieved = move();
          childs0 = lastMove()->totalChilds;
          currRating = lastMove()->rating;
        } else if (takeBackRequested) {
            takeBackRequested = false;

            if (wizardMode>0) {
                if (totalChilds > MAX_CHILDS_WIZARD) {
                        wizardMode = 0;
                } else {
                        --wizardMode;
                }
                if (wizardMode ==0) {
//                // drop to human vs comp from Comp vs comp modes if "takeback" pressed
                        if (playMode == 2) { //comp vs comp
                                restartRequested = true;
                                playMode = 1; //human vs comp
                        }
                }
            }

            if (count > 1) {
                back();
            }

            currRating = lastMove()->rating;
        }

        //********* STEP 3   autoplay stuff ***************

        if (playMode == 3 && wizardMode > 0) {//Show debuts
                if (!medRating) {
                        if (count%5 == 0 || count%7 == 0) {
                                restartRequested = true;
                        } else {
                                takeBackRequested = true;
                        }
                        ++count;
                        continue;
                }
        }

        unsigned int lastCount = lastMove()->totalChilds;
        if (wizardMode >= 0 && (playMode > 1)
                && (lastCount >= (mediumicPlay
                                        ? childPerMove
                                        : 20000))) {
                moveRequested = true;
                continue;
        }

        //********* STEP 4   tree grow ***************

        if (lastCount <  maxChilds
                        && currRating < 32300
                        && currRating > -32300) {
          buildTree();
          changed = false;
          ++count;
        } else {
          
          std::this_thread::sleep_for(std::chrono::nanoseconds(500000000));
          if (wizardMode) {

              if (mediumicPlay || playMode == 2) { //Comp vs Comp

                    moveRequested = true;
              } else
              {
                    takeBackRequested = true;
              }
              continue;
          }
        }

        //********* STEP 5   stat outputs ***************

        if (changed || !(count%100)) {
              int min = 1000000000;
              int max = 0;
              int decr;
              int i=0;

              TNode *node = lastMove();

              //begin hints calculation
              calculateChilds();
              for(i=0; i < childs.count; ++i) {
                int r = childs.node[i]->rating;
                if (r<min) min = r;
                if (r>max) max = r;
              }

              memset(dkl, 0, fsize*fsize);
              for(i=0; i<childs.count; ++i) {

                int r = childs.node[i]->rating;
                decr = min-1;
                dkl[childs.move[i]] = 30+(r-decr)*225/(max-decr);
              }
              //end hints calculation

              movesCount = count;
              int i1 = firstNode->totalChilds;
              int i2 = node->totalChilds;

              sprintf(msg1, "Childs count: %d%c / %d%c",
                            i1 / (i1 > 2000000 ? 1000000 : i1 > 2000 ? 1000 : 1), (i1 > 2000000 ? 'M' : i1 > 2000 ? 'K' : ' '),
                            i2 / (i2 > 2000000 ? 1000000 : i2 > 2000 ? 1000 : 1), (i2 > 2000000 ? 'M' : i2 > 2000 ? 'K' : ' '));

              sprintf(msg2, "Rating: %d / %d",
                        getFirstNode()->rating,
                        lastMove()->rating);

              sprintf(msg3, "Max path length: %d",
                        max_count);

              sprintf(msgStatus, wizardMode
                        ? "Please either switch playing mode or just wait.."
                        : restartRequested || moveRequested || takeBackRequested || userMoveRequested != 255
                              ? "Please wait.."
                              : "Please make your move. Or, you can give it to me, by pressing Move button.");

              logger->printLastError(msg4);
//              sprintf(msg4, movesHash->miss3 > 0 || movesHash->miss4 > 0
//                        ? "Hash collisions %d / %d" : "\0\0", movesHash->miss3, movesHash->miss4);

/*
              double updateFreq = TNode::updatesCount
                        ? 100 * TNode::updatesCount / (double)(TNode::updatesCount + TNode::skippedCount)
                        : 0;
              sprintf(msg5, "Dev: %.3f%% : %d / %d",
                        updateFreq,
                        (int)TNode::avgDiff,
                        (int)TNode::avgSquareDiff);
*/

/* TODO
              if (xo != NULL) {
                unsigned long time = GetTickCount() - beginTime;
                sprintf(msg5, "RTime: %.3f%%",
                        100 * rateTime / (float)time,
                        (int)TNode::avgDiff,
                        (int)TNode::avgSquareDiff);
              }
*/

              xRating = count%2
                ? lastMove()->rating
                : -lastMove()->rating;

        }
  }
}


void Grower::gridClick(int Col, int Row) {

  userMoveRequested = transform(Row, Col);
}

int Grower::getRResult(){

  return resultRecieved;
};

void Grower::moveClick() {

    moveRequested = true;
}

void Grower::restartClick() {

    restartRequested = true;
}


//------------------------
Grower* xo = NULL;

Grower* getXBoard(int gameMode) {

    if (xo == NULL) {

        Logger *logger = new Logger();
        SimplyNumbers *sn = new SimplyNumbers();
        Hashtable *ht = new Hashtable(logger);
        xo = new Grower(sn, ht, gameMode);
        xo->logger = logger;
    }
    return xo;
};
