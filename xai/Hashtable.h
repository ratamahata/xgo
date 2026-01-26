//---------------------------------------------------------------------------
#ifndef hashtableH
#define hashtableH
//---------------------------------------------------------------------------
#include "Hashtable.h"
#include "TNode.h"
#include "Logger.h"
#include "Persister.h"

//103x105x331 ->62 M
//53x1009x331 ->64 M

    #define hashTableSizeX 53
    #define hashTableSizeO 1009
    #define hashTableSizeZ 331
    class Hashtable {
        private:
        TNode **table;
        Logger *logger;

        public:
        Hashtable(Logger *logger);
//        void put(TNode *node);
        TNode *get(THash hX, THash hO, int age);
        TNode *getOrCreate(THash hX, THash hO, int age, bool &created);
    };

//---------------------------------------------------------------------------
#endif
