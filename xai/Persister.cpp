#include <fstream>
#include <functional>
#include "TNode.h"
#include "Persister.h"

const char* Persister::getFileName() {
    return "xo.dat";
}

void Persister::save(TNode* node) {
    if (!node) return;

    // Open in binary mode with append (app) and output (out) flags
    std::ofstream outFile(getFileName(), std::ios::binary | std::ios::app | std::ios::out);
    if (!outFile.is_open()) return;

    // Fields to save in specific order
    outFile.write(reinterpret_cast<const char*>(&node->hashCodeX), sizeof(node->hashCodeX));
    outFile.write(reinterpret_cast<const char*>(&node->hashCodeO), sizeof(node->hashCodeO));

    TByte zero = 0;
    outFile.write(reinterpret_cast<const char*>(&zero), sizeof(zero));
    outFile.write(reinterpret_cast<const char*>(&node->age), sizeof(node->age));

    outFile.write(reinterpret_cast<const char*>(&node->x3), sizeof(node->x3));
    outFile.write(reinterpret_cast<const char*>(&node->x4), sizeof(node->x4));
    outFile.write(reinterpret_cast<const char*>(&node->o3), sizeof(node->o3));
    outFile.write(reinterpret_cast<const char*>(&node->o4), sizeof(node->o4));

    TRating rating = node->rating;
    outFile.write(reinterpret_cast<char*>(&rating), sizeof(node->rating));

    outFile.close();
}


void Persister::load(std::function<TNode*(THash, THash, int)> getNode) {
    std::ifstream inFile("xo.dat", std::ios::binary);
    if (!inFile.is_open()) return;

    THash hX, hO;
    TByte dummyZero, age, x3, x4, o3, o4;
    TRating rating;

    while (inFile.read(reinterpret_cast<char*>(&hX), sizeof(hX))) {
        inFile.read(reinterpret_cast<char*>(&hO), sizeof(hO));
        inFile.read(reinterpret_cast<char*>(&dummyZero), sizeof(dummyZero));
        inFile.read(reinterpret_cast<char*>(&age), sizeof(age));
        inFile.read(reinterpret_cast<char*>(&x3), sizeof(x3));
        inFile.read(reinterpret_cast<char*>(&x4), sizeof(x4));
        inFile.read(reinterpret_cast<char*>(&o3), sizeof(o3));
        inFile.read(reinterpret_cast<char*>(&o4), sizeof(o4));
        inFile.read(reinterpret_cast<char*>(&rating), sizeof(rating));

        // Use the callback to get/create the node in the table
        TNode* targetNode = getNode(hX, hO, (int)age);

        if (targetNode) {
            // Fill the rest of the fields
            targetNode->x3 = x3;
            targetNode->x4 = x4;
            targetNode->o3 = o3;
            targetNode->o4 = o4;
            targetNode->rating = rating;
            targetNode->setFixedRating(true);
        }
    }
    inFile.close();
}