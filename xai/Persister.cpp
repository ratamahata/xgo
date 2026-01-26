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

    outFile.write(reinterpret_cast<const char*>(&node->rating), sizeof(node->rating));

    outFile.close();
}

void Persister::load(std::function<void(TNode)> processNode) {
    std::ifstream inFile(getFileName(), std::ios::binary);
    if (!inFile.is_open()) return;

    while (inFile.peek() != EOF) {
        TNode node;
        TByte dummyZero;

        // Read fields in the exact same order as saved
        inFile.read(reinterpret_cast<char*>(&node.hashCodeX), sizeof(node.hashCodeX));
        inFile.read(reinterpret_cast<char*>(&node.hashCodeO), sizeof(node.hashCodeO));

        inFile.read(reinterpret_cast<char*>(&dummyZero), sizeof(dummyZero));
        inFile.read(reinterpret_cast<char*>(&node.age), sizeof(node.age));

        inFile.read(reinterpret_cast<char*>(&node.x3), sizeof(node.x3));
        inFile.read(reinterpret_cast<char*>(&node.x4), sizeof(node.x4));
        inFile.read(reinterpret_cast<char*>(&node.o3), sizeof(node.o3));
        inFile.read(reinterpret_cast<char*>(&node.o4), sizeof(node.o4));

        inFile.read(reinterpret_cast<char*>(&node.rating), sizeof(node.rating));

        if (inFile) {
            processNode(node);
        }
    }

    inFile.close();
}

