//---------------------------------------------------------------------------

#ifndef PersisterH
#define PersisterH
//---------------------------------------------------------------------------
#include <functional>
#include "TNode.h"


class Persister {

private:
    const char* getFileName();

public:
    void save(TNode* node);
    void load(std::function<TNode*(THash, THash, int)> getNode);

};

#endif
