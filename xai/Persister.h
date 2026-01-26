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
    void load(std::function<void(TNode)> processNode);

};

#endif
