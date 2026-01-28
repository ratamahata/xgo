//---------------------------------------------------------------------------

#ifndef ExpanderH
#define ExpanderH
//---------------------------------------------------------------------------

#include "Relator.h"

class MovesBucket {
        public:
        int count;
        TMove move[MAX_RELATIVES];
};

class Expander : public Relator {

public:
        int max_count;

protected:
        Expander(SimplyNumbers *simplyGen, Hashtable *movesHash);
        void expand();
//        TNode* createNode(THash hX, THash hO, TByte age);        
private:
        MovesBucket newChilds;
        void findMovesToExpand(int start);
};

#endif
