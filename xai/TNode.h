//---------------------------------------------------------------------------
#ifndef tnodeH
#define tnodeH
//---------------------------------------------------------------------------
#include "TNode.h"

#include <stdint.h>
#include <cstddef>

//#define NULL 0

typedef unsigned char TByte;
typedef TByte TMove;
typedef uint32_t THash;
#define THASH_MAX 4294967295U //ULONG_MAX from <limits.h>

#define BIG_PARENT1    9000
#define BIG_PARENT2   18000
#define BIG_PARENT3   36000
#define BIG_PARENT4   72000
#define BIG_PARENT5  144000

#define CULL_RATING1 8000
#define CULL_RATING2 16000

typedef signed short int TRating;

    class TNode {
      public:
      volatile static double avgDiff, avgSquareDiff;
      volatile static long long updatesCount;
      volatile static long long skippedCount;
      volatile static long long maxUpdated;
      volatile static TNode* first;

      TNode();
      void update(short int newRating, unsigned int addedChilds);
      int ratingToTotalChilds();
//      int removeChild(TNode *child);
//      void remove();
//      int checkChilds(int recursion);
//      int cleanChilds(int critRating, int recursion);


      unsigned int totalChilds;
      volatile TRating rating;
//      TNode *firstParent;//deprecated
//      TNodeLink2 *nextParent;//deprecated
//      TNodeLink *childs;//deprecated
//      void *reserved;
      bool fixedRating;
      TByte //move,
        totalDirectChilds,
        x2,x3,x4,//checks after this move of opponent
        o2,o3,o4,//checks after this move of move-maker
        age;

      THash hashCodeX, hashCodeO;
      TNode *next;
    };


//---------------------------------------------------------------------------
#endif