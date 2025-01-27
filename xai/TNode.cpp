#include "TNode.h"

TNode::TNode() {
    totalChilds = 0;
    rating = 0;
    totalDirectChilds = 0;
    hashCodeX = 1;
    hashCodeO = 1;
    x2 = x3 = x4 = o2 = o3 = o4 = 0;
    fixedRating = false;
    next = 0;
};


/*
//==================================================================
int TNode::removeChild(TNode *child) {
        int found = 0;
        for(int i=0; i<totalDirectChilds; ++i) {
                if (childs[i].node == child) {
                        ++found;
                } else if (found > 0) {
                        childs[i-found].node = childs[i].node;
                        childs[i-found].move = childs[i].move;
                }
        }
        if (totalDirectChilds > found) {
                totalDirectChilds -= found;
        } else {
                totalDirectChilds = 0;
                delete[] childs;
        }
        return 0;
}
//==================================================================
void TNode::remove() {
        firstParent->removeChild(this);
        TNodeLink2** next = &nextParent;
        while((*next) != NULL) {
                (*next)->node->removeChild(this);
                next = &((*next)->next);
        }
}

//==================================================================

int TNode::cleanChilds(int critRating, int recursion) {
  int cleaned = 0;
  for(int i=0; i < totalDirectChilds; ++i) {
        int c = childs[i].node->cleanChilds(critRating, recursion+1);
        if (c > 0 && childs[i].node->totalDirectChilds == 0) {
                delete childs[i].node;
        }
        cleaned += c;
  }
  if (recursion > 0 && totalDirectChilds == 0) {
        if (rating > -critRating && rating < critRating) {
                return 0;
        }
        remove();
        ++cleaned;
  }
  return cleaned;
}

//==================================================================

int TNode::checkChilds(int recursion) {
  if (!totalDirectChilds) {
        return 0;
  }
  int violationsCount = 0;
  int maxR = -33000;
  for(int i=0; i < totalDirectChilds; ++i) {
        if (childs[i].node->rating > maxR) {
                maxR = childs[i].node->rating;
        }
  }
  maxR = -maxR;

  float k = maxR == rating
        ? 1
        : rating/ (float)(maxR ? maxR : 1);
  if (k < 0.9 || k > 1.1) {
        ++violationsCount;
  }

  for(int i=0; i < totalDirectChilds; ++i) {
        violationsCount += childs[i].node->checkChilds(recursion+1);
  }
  return violationsCount;
}
  */

  
//==================================================================

int TNode::ratingToTotalChilds() {

//  if (age == 1 && rating < -firstNode->rating*1.01 && hashCodeX == 1 && hashCodeO <=3
//        && firstNode->rating > 0 && this != firstNode) {
//    rating = -firstNode->rating*1.01;
//  }

  if (rating < -20000) {
    return 1;
  }
  if (rating < -10000) {
    return 2 + (rating+20000)/1000;//2..12
  }

  if (rating < -4500) {
    return 13 + (rating+10000)/200;//13..40
  }

  if (rating < -2000) {
    return 41 + (rating+4500)/10;//41..291
  }

  if (rating < 20000) {
    return 292 + (rating+2000);//292..22292
  }

  return 22292;
}




//==================================================================

void TNode::update(short int newRating, unsigned int addedChilds) {
    totalChilds += addedChilds;

    if (rating != newRating) {

        if (rating == 0) {
            rating = newRating;
        } else if ((rating <= - 16000 && newRating > rating) ||
                (rating >= 16000 && newRating < rating)) {
                //suppress decreasing of rating
//                if (totalChilds > 150000) {
//                        ++TNode::skippedCount;
//                }
        } else {
                if (totalChilds > 150000) {
                    double diff = (newRating - rating)*100.0;
                    double sqDiff = diff > 0 ? diff : -diff;
//                    avgDiff = (avgDiff*updatesCount + diff)/(updatesCount+1);
//                    avgSquareDiff = (avgSquareDiff*updatesCount + sqDiff)/(updatesCount+1);
//                    ++updatesCount;
                }

                if (newRating > 16384) {
                        rating = newRating - 1;
                } else if (newRating < -16384) {
                        rating = newRating + 1;
//                } else if (totalChilds < 50000) {
                } else {
                        if (!fixedRating) {
                                rating = newRating;
                        }
//                } else if (totalChilds < 150000) {
//                        rating = (short int)floor(0.5 + 0.99*newRating + 0.01*rating);
//                } else {
//                        rating = (short int)floor(0.5 + 0.98*newRating + 0.02*rating);
                }
        }
    } else {
        if (totalChilds > 150000) {
//                ++skippedODCount;
        }
    }
};
