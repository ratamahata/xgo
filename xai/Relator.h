//---------------------------------------------------------------------------

#ifndef RelatorH
#define RelatorH
//---------------------------------------------------------------------------
#include "Evaluator.h"
#include "TNode.h"
#include "Persister.h"

class Relator : public Evaluator {

        public:
        void calculateChilds();
        RelativeBucket childs;

        void updateParents(int addedChilds);        
//-------------------------------------
        protected:

        Relator(SimplyNumbers*, Hashtable* );

        Persister *persister;

        TNode* getChild(TNode *parent, TMove childMove);
        RelativeBucket getParents(TNode *node);
        bool isPerspectiveChild(TMove move);
        bool isPerspectiveChildMode1(TMove move);

        bool updateNode(TNode *node, TNode *from, bool updateRating, int addedChilds, int removedFromEnd);        


//-------------------------------------
        private:

        //these are used fro parent calculation
//        void updateParents(int childsAdded);
        void updateParents(TNode *node, int removed, int removedFromEnd,
                bool onlyLastRemoved, bool updateRating, int max, int addedChilds);


        TNode* getParent(TNode *node, TMove move);
        int minRemovedEven;
        int minRemovedOdd;
};

#endif
