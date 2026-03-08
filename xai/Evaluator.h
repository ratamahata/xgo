//---------------------------------------------------------------------------

#ifndef EvaluatorH
#define EvaluatorH

#include "SimplyNumbers.h"
#include "Hashtable.h"
#include "Cursor.h"
//---------------------------------------------------------------------------

class Evaluator : public Cursor {

        private:
        int cnt;
        inline bool comp(int x, int y, unsigned char c);
        void addAttackPair(TNode* destNode, int x1, int y1, int x2, int y2, int &totalAttacks);

        protected:
        bool  scanlines(int BlNo, int &lines, int N);
        int scanlines(int BlNo, int &lines, int N, TNode *destNode, int &nAttacks);

        public:
        Evaluator(SimplyNumbers* sn, Hashtable* ht);
        void rate(TNode *src, TNode *destNode, TMove move); //fills {totalRating,x3,x4,o3,o4} of dest;
};
#endif
