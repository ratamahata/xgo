#include <memory.h>
#include "Hashtable.h"

//------------------------------------------------------------------
Hashtable::Hashtable(Logger *logger) {
    // Initialize the table array
    table = new TNode*[hashTableSizeX * hashTableSizeO];
    memset(table, 0, sizeof(TNode*) * hashTableSizeX * hashTableSizeO);
    this->logger = logger;

    Persister *persister = new Persister();

    // Pass a lambda that calls getOrCreate and returns the pointer
    persister->load([this](THash hX, THash hO, int age) -> TNode* {
        bool created = false;
        TNode* node = this->getOrCreate(hX, hO, age, created);
        return node;
    });
}
/*
//------------------------------------------------------------------
void Hashtable::put(TNode *node) {

    THash hashX = node->hashCodeX;
    unsigned long h1 = hashX / hashTableSizeZ;
    unsigned long zIndex = hashX % hashTableSizeZ;
    h1 = h1 % hashTableSizeO;
    unsigned long h2 = node->hashCodeO % hashTableSizeO;
    unsigned long index = h1*hashTableSizeO + h2;

    TNode *array = table[index];
    if (array == NULL) {
        table[index] = array = new TNode[hashTableSizeZ];
        memset(array, 0, sizeof(TNode)*hashTableSizeZ);
    }

    TNode *curr = &(array[zIndex]);

    while (curr->hashCodeX != 0) {
//        if (array[0]->hashCodeX == node->hashCodeX && array[0]->hashCodeO == node->hashCodeO) {
//          return;
//        }
        curr = &(curr[0]->next);
    }
    curr[0] = node;
}
*/

//------------------------------------------------------------------
TNode *Hashtable::getOrCreate(THash hX, THash hO, int age, bool &created) {
    THash t1 = hO - 1;
    THash t2 = hX - 1;
    unsigned long h1 = t1 / hashTableSizeZ;
    unsigned long zIndex = t1 % hashTableSizeZ;
    h1 = h1 % hashTableSizeX;
    unsigned long h2 = t2 % hashTableSizeO;
    unsigned long index = h1*hashTableSizeO + h2;

    TNode *array = table[index];
    bool m2 = false;
    bool m3 = false;

//    if (hX ==0 || hO ==0) {
//        logger->error("zero hashcode");
//    }

    if (array == NULL) {
        table[index] = array = new TNode[hashTableSizeZ];
        memset(array, 0, sizeof(TNode)*hashTableSizeZ);
    }

    TNode *curr = &(array[zIndex]);

    while (curr->hashCodeX != 0) {
        if (curr->hashCodeX == hX && curr->hashCodeO == hO) {
          if (age == curr->age) {
                logger->hit();
                created = false;
                return curr;
          } else {
                m3 = true;
                logger->error("age collision");
          }
        } else {
                m2 = true;
        }

        if (curr->next == NULL) {
                curr = curr->next = new TNode();
                break;
        } else {
                curr = curr->next;
        }
    }

    curr->hashCodeX = hX;
    curr->hashCodeO = hO;
    curr->age=age;
    created = true;

    if (m3) {
        logger->missAge();
    } else if (m2) {
        logger->missHash();
    } else {
        logger->missIndex();
    }
    return curr;
};
//------------------------------------------------------------------
TNode *Hashtable::get(THash hX, THash hO, int age) {
    THash t1 = hO - 1;
    THash t2 = hX - 1;
    unsigned long h1 = t1 / hashTableSizeZ;
    unsigned long zIndex = t1 % hashTableSizeZ;
    h1 = h1 % hashTableSizeX;
    unsigned long h2 = t2 % hashTableSizeO;
    unsigned long index = h1*hashTableSizeO + h2;

    TNode *array = table[index];
    bool m2 = false;
    bool m3 = false;

    if (hX ==0 || hO ==0) {
        logger->error("zero hashcode");
    }

    if (array != NULL) {
      TNode *curr = &(array[zIndex]);
      while (curr != NULL) {
        if (curr->hashCodeX == hX && curr->hashCodeO == hO) {
          if (age == curr->age) {
                        logger->hit();
                        return curr;
          } else {
                m3 = true;
          }
        } else {
                m2 = true;
        }
        curr = curr->next;
      }
    }

    if (m3) {
        logger->missAge();
    } else if (m2) {
        logger->missHash();
    } else {
        logger->missIndex();
    }
    return NULL;
};



