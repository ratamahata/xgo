//---------------------------------------------------------------------------

#ifndef ExpanderH
#define ExpanderH
//---------------------------------------------------------------------------

#include "Relator.h"

class MovesBucket {
        public:
        int count;
        TMove move[MAX_RELATIVES];
        void add(TMove m);
        bool contains(TMove m);
};

class Expander : public Relator {

public:
        int max_count;

protected:
        Expander(SimplyNumbers *simplyGen, Hashtable *movesHash);
        void expand(int start, TNode* cursor);
        void fullExpand(TNode* cursor);
//        TNode* createNode(THash hX, THash hO, TByte age);        
private:
        int cnt;
        MovesBucket newChilds;
        MovesBucket otherNewChilds; //second priority
        void findMovesToExpand(int start);
        void addChildNoDupe(TNode* parent, TMove m);
        void addChild(TNode* parent, TMove m);
        bool isExpected(TNode* curr, TMove i);
};

#endif
