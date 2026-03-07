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
        void addAttackIfEmpty(TNode* destNode, int cx, int cy);

        protected:
        bool  scanlines(int BlNo, int &lines, int N);
        bool  scanlines2(int BlNo, int &lines, int N);
        int   scanlines(int BlNo, int &lines, int N, TNode *destNode);

        public:
        Evaluator(SimplyNumbers* sn, Hashtable* ht);
        void rate(TNode *src, TNode *destNode, TMove move); //fills {totalRating,x3,x4,o3,o4} of dest;
};
#endif
