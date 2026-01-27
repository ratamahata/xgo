//---------------------------------------------------------------------------


#pragma hdrstop

#include "Expander.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Expander::Expander(SimplyNumbers *simplyGen, Hashtable *movesHash)
        : Relator (simplyGen, movesHash) {
  max_count = 0;
}

//==================================================================
//adds childs to cursor node
void Expander ::expand() {
  TNode* cursor = current()->node;
//  if (cursor->rating >= 32300 || cursor->rating <= -32300) {
  if (cursor->rating > 32200 || cursor->rating < -32200) {
      ++cursor->totalChilds;
      //logger->error("can't expand");
      return;
  }
//  if (logger != NULL) {
//        logger->expand(count);
//  }
  findMovesToExpand();

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
        cursor->totalChilds += (node->totalChilds+1);
    }

    if (node->rating > max_rating) max_rating = node->rating;
  }

  short int oldRating = cursor->rating;
  if (!cursor->fixedRating) {
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

/*
//==================================================================
TNode* Expander ::createNode(THash hX, THash hO, TByte age) {
    TNode *node = new TNode();
    node->hashCodeX = hX;
    node->hashCodeO = hO;
    node->age = age;
    movesHash->put(node);
    return node;
}
*/

//----------------------------------------------------------------------------

void Expander::findMovesToExpand() {//TODO use single iteration
    newChilds.count = 0;
    bool mode1 = gameMode == 1 &&  count == 2;
    int t;
    TNode* curr = current()->node;

    for (int pass = 0; pass < 2; ++pass) {
        for (TMove i = 0; i < TOTAL_CELLS; ++i) {
            if ((mode1 ? kl[i]<=1 && isPerspectiveChildMode1(i) : isPerspectiveChild(i))  && isAlllowed(i)) {

                if (pass == 0) {

                    t = 15;

                    if (curr->x4 > 0) {
                        if (scanlines(0, t, i) > 0) {
                            //logger->log("FIVE");
                        } else {
                            continue;
                            //logger->log("FIVE MISS");
                        }
                    }

                }
                newChilds.move[newChilds.count++] = i;
            }
        }
        if (pass == 0) {
            if (curr->x4 > 0) {
                if (newChilds.count > 0) {
                    logger->log("FIVE");
                } else {
                    logger->log("FIVE MISS");
                }
            }
        }
        if (newChilds.count > 0) return;
    }
};
