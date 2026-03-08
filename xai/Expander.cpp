//---------------------------------------------------------------------------
#include <iostream>


#pragma hdrstop

#include "Expander.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Expander::Expander(SimplyNumbers *simplyGen, Hashtable *movesHash)
        : Relator (simplyGen, movesHash) {
  max_count = 0;
  cnt = 0;
}

//==================================================================
void Expander ::fullExpand(TNode* cursor) {

  if (!cursor->isRage() || cursor->totalChilds < 150000)  return;
  cursor->setRage(false);

  findMovesToExpand(1);

  if (newChilds.count == 0) {
    return;
  }

  TRating max_rating = -32600;

  int created = 0;

  for(int i=0; i<newChilds.count; ++i) {

        TMove move = newChilds.move[i];

        THash newHashX = cursor->hashCodeO;
        THash newHashO = cursor->hashCodeX * simplyGen->getHash(move);

        bool isCreated;
        TNode *node = movesHash->getOrCreate(newHashX, newHashO, cursor->age + 1, isCreated);

        if (isCreated) {
            ++created;
            rate(cursor, node, move);
            ++cursor->totalChilds;
            ++cursor->totalDirectChilds;
        }

        if (node->rating > max_rating) max_rating = node->rating;
  }

  short int oldRating = cursor->rating;
  if (!cursor->isFixedRating()) {
        cursor->rating = (TRating)-max_rating;
  }
  if (created == 0 && oldRating == cursor->rating) return;

  updateParents(created);
}

//adds childs to cursor node
void Expander ::expand(int startPass, TNode* cursor) {

  if (cursor->rating > 32500 || cursor->rating < -32500) {
      cursor->totalChilds += 100;
      updateParents(100);
      logger->missExpand(cursor);
      return;
  }
//  if (logger != NULL) {
//        logger->expand(count);
//  }
  findMovesToExpand(startPass);

  if (newChilds.count == 0) {
    logger->missMoves(cursor);
    return;
  }

  TRating max_rating = -32600;

  int created = 0;
  cursor->totalChilds = 0;
  for(int i=0; i<newChilds.count; ++i) {

    TMove move = newChilds.move[i];

    THash newHashX = cursor->hashCodeO;
    THash newHashO = cursor->hashCodeX * simplyGen->getHash(move);

    bool isCreated;
    TNode *node = movesHash->getOrCreate(newHashX, newHashO, cursor->age + 1, isCreated);

    if (isCreated) {
        ++created;
        rate(cursor, node, move);
        ++cursor->totalChilds;
    } else {
        if (startPass == 0) {
            cursor->totalChilds += (node->totalChilds+1);
        }
    }

    if (node->rating > max_rating) max_rating = node->rating;
  }

  short int oldRating = cursor->rating;
  if (!cursor->isFixedRating()) {
        cursor->rating = (TRating)-max_rating; //(TRating)(0.4*(double)oldRating-0.6*(double)max_rating);
  }

  cursor->totalDirectChilds = newChilds.count;

  updateParents(created);
// TODO:
//  updatedParentsCounter = 0;
//  updateParents(cursor, cursor->totalChilds, oldRating);
//  current()->

  if (count > max_count) max_count = count;
};

//----------------------------------------------------------------------------
/*
void Expander::findMovesToExpand(int startPass) {//TODO use single iteration
    newChilds.count = 0;
    bool mode1 = gameMode == 1 &&  count == 2;
    int t;
    TNode* curr = current()->node;

//    if (curr->x4 > 0) {
//            logger->log("FIVE?");
//    } else if (curr->o4 > 0) {
//           logger->log("CLOSE4?");
//    } else if (curr->x3 > 0) {
//           logger->log("FOUR?");
//    } else if (curr->o3 > 0) {
//           logger->log("CLOSE3?");
//    }

   bool rage = false;

    for (int pass = startPass; pass < 2; ++pass) {
        for (TMove i = 0; i < TOTAL_CELLS; ++i) {

            if ((mode1 ? kl[i]<=1 && isPerspectiveChildMode1(i) : isPerspectiveChild(i))  && isAlllowed(i)) {

                if (pass == 0) {

                    t = 15;

                    if (curr->x4 > 0) {
                        if (scanlines(0, t, i) <= 0) {
                            continue;//filter out nodes which not allows to build 5
                        }
                    } else if (curr->o4 > 0) {
                        if (scanlines(1, t, i) <= 0) {
                            continue;//filter out nodes which not allows to close 4
                        }
                    } else if (curr->x3 > 0) {
                        if (scanlines(2, t, i) <= 0) {
                            continue;//filter out nodes which not allows to build opened 4
                        }
                    } else if (curr->o3 > 0) {
                        if (scanlines(3, t, i) <= 0 && scanlines(4, t, i) <= 0) {
                            continue;//filter out nodes which allows neither close 3 nor build closed  4
                        }
                    }
//                     else if (curr->x2 > 0 && (curr->totalDirectChilds == 0 || curr->rating > 2400)) {
//                        if (scanlines(4, t, i) <= 0 && scanlines(5, t, i) <= 0) {
//                            rage = true;
//                            continue;//filter out nodes which not allows to build 3 or 4
//                        }
//                    }

                }
                newChilds.move[newChilds.count++] = i;
            }
        }
        if (pass == 0) {
            if (newChilds.count == 0) {
                if (curr->x4 > 0) {
                   logger->miss5();
                } else if (curr->o4 > 0) {
                   logger->miss4o();
                } else if (curr->x3 > 0) {
                   logger->miss4();
                } else if (curr->o3 > 0) {
                   logger->miss3o();
                } else if (curr->x2 > 0 && (curr->totalDirectChilds == 0 || curr->rating > 2400)) {
                   logger->miss3();
                }
            }
//            else {
//                if (curr->x2 > 0 && (curr->totalDirectChilds == 0 || curr->rating > 2400) && rage) {
//                    //rage mode: number of childs was reduced to only those which builds 3 or 4
//                    curr->setRage(true);
//                }
//            }
        }
        if (newChilds.count > 0) return;
        //logger->log("NEXT PASS");
    }
};
*/


void Expander::findMovesToExpand(int startPass) {
    newChilds.count = 0;
    bool mode1 = (gameMode == 1 && count == 2);
    CursorHistory *h = current();
    TNode* curr = h->node;

    if (startPass == 0) {
        bool forceAttack = curr->x4 > 0
            || (curr->x3 > 0 && curr->o4 == 0)
            || (curr->x2 > 0 && curr->o4 == 0 && curr->o3 == 0 && (curr->totalDirectChilds == 0 || curr->rating > 2400));

        bool forceDefense = (curr->o4 > 0 || curr->o3 > 0);

        if (curr->hashCodeX==34 && curr->hashCodeO==133) {

            std::cout << "34_133 A/D: " << forceAttack << " " << forceDefense;
        }

        if (forceAttack || forceDefense) {
            int startIdx = forceDefense ? 0 : curr->ownAttacks;
            int endIdx = forceDefense ? curr->ownAttacks : MAX_ATTACK_2;

            for (int i = startIdx; i < endIdx; ++i) {
                TAttack &atk = curr->attacks[i];
                if (atk.l == 0 && atk.r == 0) break;

                // 1. Вычисляем вектор направления между l и r
                int x1 = atk.l % fsize, y1 = atk.l / fsize;
                int x2 = atk.r % fsize, y2 = atk.r / fsize;

                int dx = (x2 > x1) ? 1 : (x2 < x1 ? -1 : 0);
                int dy = (y2 > y1) ? 1 : (y2 < y1 ? -1 : 0);

                // 2. Проходим от l до r включительно
                int curX = x1, curY = y1;
                while (true) {
                    TMove m = (TMove)(curX + curY * fsize);

                    // Проверка: клетка должна быть пустой (kl[m] & 3 == 0) и разрешенной
                    if ((kl[m] & 12) == 0) {
                        // Проверка на дубликаты в newChilds
                        bool alreadyAdded = false;
                        for (int k = 0; k < newChilds.count; ++k) {
                            if (newChilds.move[k] == m) { alreadyAdded = true; break; }
                        }

                        if (!alreadyAdded) {
                            if ((mode1 ? kl[m] <= 1 && isPerspectiveChildMode1(m) : isPerspectiveChild(m)) && isAlllowed(m)) {
                                if (newChilds.count < MAX_RELATIVES) {
                                    newChilds.move[newChilds.count++] = m;
                                }
                            }
                        }
                    }

                    if (curX == x2 && curY == y2) break;
                    curX += dx; curY += dy;
                    if (curX < 0 || curX >= fsize || curY < 0 || curY >= fsize) break; // На всякий случай
                }
            }

            if (newChilds.count > 0) {
                curr->setRage(true);
                return;
            }

            // Логирование промахов...
            if (curr->x4 > 0) logger->miss5();
            else if (curr->o4 > 0) logger->miss4o();
            else if (curr->x3 > 0) logger->miss4();
            else if (curr->o3 > 0) logger->miss3o();
            else if (curr->x2 > 0 && (curr->totalDirectChilds == 0 || curr->rating > 2400)) logger->miss3();
        }
    }

    // 2. Общий сбор ходов (выполняется если атак нет)
    for (TMove i = 0; i < TOTAL_CELLS; ++i) {
        if ((mode1 ? kl[i] <= 1 && isPerspectiveChildMode1(i) : isPerspectiveChild(i)) && isAlllowed(i)) {
            newChilds.move[newChilds.count++] = i;
        }
    }
}
