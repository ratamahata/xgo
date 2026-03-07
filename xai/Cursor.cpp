//---------------------------------------------------------------------------


//#pragma hdrstop

#include <iostream>
#include <memory.h>
#include "Cursor.h"


//---------------------------------------------------------------------------

//#pragma package(smart_init)

CursorHistory::CursorHistory() {
        memset((void*)en, 0, sizeof(CursorHistory));
};

#include <cstdio> // Для snprintf

void CursorHistory::printAttacks(char* buffer, size_t size) {
    if (size == 0) return;

    char* ptr = buffer;
    size_t remaining = size;
    int written = 0;

    // Функция-помощник для печати одного массива
    auto printArray = [&](const char* label, TMove* arr, int count) {
        // Печатаем заголовок (например, "X: ")
        written = snprintf(ptr, remaining, "%s: ", label);
        if (written > 0 && (size_t)written < remaining) {
            ptr += written;
            remaining -= written;
        }

        for (int i = 0; i < count && arr[i]; ++i) {
            // Кастуем каждый элемент TMove к int и печатаем через запятую
            written = snprintf(ptr, remaining, "%d%s", (int)arr[i], (i == count - 1 ? "" : ", "));

            if (written > 0 && (size_t)written < remaining) {
                ptr += written;
                remaining -= written;
            } else {
                break; // Буфер переполнен
            }
        }

        written = snprintf(ptr, remaining, " ");
        if (written > 0 && (size_t)written < remaining) {
            ptr += written;
            remaining -= written;
        }
    };

    printArray("A.X", attacksX, attacksXcount);
    printArray("A.O", attacksO, attacksOcount);
}

//-------------------------------  CURSOR -----------------------------------

Cursor::Cursor(SimplyNumbers *simplyGen, Hashtable *movesHash) {
        this->simplyGen = simplyGen;
        this->movesHash = movesHash;
        memset(history,0,sizeof(CursorHistory)*TOTAL_CELLS);
        count = 0;
        count0 = 0;
        building = false;
        cnt = 0;
};

//=============================================================================

//inline
CursorHistory *Cursor::current() {
        return &(history[count-1]);
}

CursorHistory *Cursor::getMove(int i) {
        return &(history[i]);
}

TNode *Cursor::getFirstNode() {
        return history[0].node;
}

TNode *Cursor::lastMove() {
        return current()->node;
}

void Cursor::restart() {
  while(count > 1) {
     back();
  }
  //lastmove = first->node;
}



//=============================================================================
//Increment the Cursor to next move N. Returns true on success, otherwise - false.
bool Cursor::forward(TMove N) {

  TNode *node = current()->node;

  THash hashCodeX = node->hashCodeO;
  THash hashCodeO = node->hashCodeX * simplyGen->getHash(N);

  bool created = false;
  TNode *nextNode = movesHash->getOrCreate(hashCodeX, hashCodeO, node->age + 1, created);
  if (nextNode == NULL) {
    logger->log("Error creating node.");
    return false;
}
  if (created) {

    rate(node, nextNode, N);
    ++node->totalChilds;

    logger->missNode(nextNode);
    logger->log("Warning: not existed node.");
  }

  return forward(N, nextNode);
}

//=============================================================================
bool Cursor::forward(TMove N, TNode* node) {


  int x = N % 15, y = N / 15;

  //begin: forward cursor
  TByte prevVal = kl[N];
  kl[N] = ((count&1)+1)<<2;
  ++count;
  moves [count] = N;

  CursorHistory *curr = current(), *prev = &(history[count <= 1? 0 : count-2]);
  curr->node = node;
  curr->move = N;
  curr->previousKlValue = prevVal;


    // 1. Наследование и Своп (атаки противника становятся моими и наоборот)
    if (prev) {
        curr->attacksXcount = prev->attacksOcount;
        curr->attacksOcount = prev->attacksXcount;
        for (int i = 0; i < curr->attacksXcount; ++i) curr->attacksX[i] = prev->attacksO[i];
        for (int i = 0; i < curr->attacksOcount; ++i) curr->attacksO[i] = prev->attacksX[i];
    } else {
        curr->attacksXcount = 0;
        curr->attacksOcount = 0;
    }

    // 2. Удаление хода N (замена последним элементом + выход)
    for (int i = 0; i < curr->attacksXcount; ++i) {
        if (curr->attacksX[i] == N) {
            curr->attacksX[i] = curr->attacksX[curr->attacksXcount - 1];
            curr->attacksXcount--;
            break;
        }
    }
    for (int i = 0; i < curr->attacksOcount; ++i) {
        if (curr->attacksO[i] == N) {
            curr->attacksO[i] = curr->attacksO[curr->attacksOcount - 1];
            curr->attacksOcount--;
            break;
        }
    }

    // 3. Добавление новых атак из TNode в attacksO (атаки текущего игрока)
    if (node) {
        for (int i = 0; i < MAX_ATTACK; ++i) {
            TMove newAttack = node->attacks[i];
            if (newAttack == 0) break;
            bool duplicate = false;
            for(int j = 0; j < curr->attacksOcount; ++j) {
                if(curr->attacksO[j] == newAttack) { duplicate = true; break; }
            }
            if (!duplicate && curr->attacksOcount < MAX_HISTORY_ATTACKS) {
                curr->attacksO[curr->attacksOcount++] = newAttack;
//                if (cnt<10) {
//                    std::cout << " At:" << (int)newAttack;
//                    std::cout << std::endl << " (`X,Y):" << node->hashCodeX << " " << node->hashCodeO << std::endl;
//                    ++cnt;
//                }
            }
        }

    }


  //begin: update simmetries history
        int d = 0,i;
        d = x==7 ? 0 : (kl[y*15+(14-x)] == kl[N]) ? -1 : 1;
        curr->symmX = prev->symmX + d;

        d = y==7 ? 0 : (kl[(14-y)*15+x] == kl[N]) ? -1 : 1;
        curr->symmY = prev->symmY + d;

        i = (14-y)*15+(14-x);
        d = N==i? 0 : (kl[i] == kl[N]) ? -1 : 1;
        curr->symmXY = prev->symmXY + d;

        d = x==y ? 0 : (kl[x*15+y] == kl[N]) ? -1 : 1;
        curr->symmW = prev->symmW + d;

        i=(14-x)*15+y;
        d = N==i? 0 : (kl[i] == kl[N]) ? -1 : 1;
        curr->symmXW = prev->symmXW + d;

        i=x*15+14-y;
        d = N==i? 0 : (kl[i] == kl[N]) ? -1 : 1;
        curr->symmYW = prev->symmYW + d;

        i=(14-x)*15+14-y;
        d = N==i? 0 : (kl[i] == kl[N]) ? -1 : 1;
        curr->symmXYW = prev->symmXYW + d;
  //end: update symmetries history

  //begin: update "enablers" history
    curr->enCount = 0;
/*
    if (gameMode == 1 &&  count == 2) {
        for (int x1=4; x1<=10; x1++)
            for (int y1=4; y1<=10; y1++)
                if (x1==4||x1==10||y1==4||y1==10) {
                        TMove move = y1*fsize+x1;
                        if (kl[move] == 0) {
                            if (curr->enCount >= MAX_ENABLERS) {
                                curr->enCount = curr->enCount; //TODO
                            } else {
                                kl[move] = 1;
                                curr->en[curr->enCount++] = move;
                            }
                        }
                }
    } else if (gameMode == 1 &&  count == 3) {
        enable(curr, x, y, 1);
        enable(curr, 7, 7, 1);
        enable(curr, history[1].move%15, history[1].move/15, 1);
    } else */


    int max = count > 3 ? 2 : 1;
    enable(curr, x, y, max);

  //end: update "enablers" history

  if (current()->node == NULL) {
        return false;
  }

  return true;
};

//=============================================================================

void Cursor::enable(CursorHistory *curr, int x, int y, int maxDistance) {
        for (int d=1; d<=maxDistance; d++)
            for (int i=-1; i<=1; i++)
              for (int j=-1; j<=1; j++)
                if ((!i)&&(!j)) continue;
                else {
                    int x1 = x+i*d, y1 = y+j*d;
                    TMove move = y1*fsize+x1;
                    if (x1>=0 && y1>=0 && x1<fsize && y1<fsize && kl[move] == 0 ) {
                          if (curr->enCount >= MAX_ENABLERS) {
                              curr->enCount = curr->enCount; //TODO
                          } else {
                              kl[move] = 1;
                              curr->en[curr->enCount++] = move;
                        }
                    }
                }

}

//set cursor to one move back
bool Cursor::back() {
  if (count == 0) {
        return false;
  }
  CursorHistory *curr = current();
  kl[curr->move] = curr->previousKlValue;
  for(int t=0; t<curr->enCount; t++)
        kl[curr->en[t]] = 0;
  --count;
  if (current()->node == NULL) {
        return false;
  }
  return true;
}

//=============================================================================

bool Cursor::isAlllowed(TMove N) {
    int x = N%fsize - 7;
    int y = N/fsize - 7;
    if ((  history[count-1].symmX  == 0) && x < 0) {
      return false;
    }
    if ((  history[count-1].symmY  == 0 || history[count-1].symmXY  == 0) && y < 0) {
      return false;                }
    if ((  history[count-1].symmW  == 0) && x < y) {
      return false;
    }
    if ((  history[count-1].symmXYW  == 0) && x < -y) {
      return false;
    }
    return allow(N);
};


//------------------------------------------------------------------------------

//bool Cursor::unique(TMove move) {
//        if (history[count-1].symmX && history[count-1].symmY && history[count-1].symmXY && history[count-1].symmW
//                //&& history[count-1].symmXW && history[count-1].symmYW && history[count-1].symmXYW
//                ) {
//                return childCount;
//        }

//                int x = move%fsize - 7;
//                int y = move/fsize - 7;

//                return (history[count-1].symmX  != 0 || x >= 0)
//                              && (history[count-1].symmY  != 0 || y >= 0)
//                              //&& (history[count-1].symmXY != 0 || x + y >= 0)
//                              && (history[count-1].symmW != 0 || x >= y)
//                              //&& (history[count-1].symmXW  != 0 || ((x>=0 || y<=0) && (x<=0 || y>=)0))
//                              //&& (history[count-1].symmYW != 0 || ((x>=0 || y<=0) && (x<=0 || y>=0)))
//                              && (history[count-1].symmXYW != 0 || x + y  >= 0)
//                              && (allow(move));
//}

//------------------------------------------------------------------------------

//initial implementation for supporting of prohibited moves for
//balanced games, currently supported 1st player
//to make 2nd move in central 5x5 square
inline bool Cursor::allow(int move) {
  if (gameMode == 1 && this->lastMove()->age == 1) {
        int x = move % 15 - 7, y = move / 15 - 7;
        return ! (x < 3 && x > -3 && y < 3 && y > -3);
  }
  return true;
}


//------------------------------------------------------------------------------

int Cursor::getMovesCount() {
  return count0 > 0 && count0<=count ? count0 : count;
}