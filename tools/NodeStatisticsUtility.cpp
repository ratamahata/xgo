#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "TNode.h" // Ensure TNode, TByte, TRating, etc., are defined here

class NodeStatisticsUtility {
public:
    void processFile() {
        const char* fileName = "xo.dat";
        std::ifstream inFile(fileName, std::ios::binary);

        if (!inFile.is_open()) {
            std::cerr << "Could not open " << fileName << " for reading." << std::endl;
            return;
        }

        // Data structures to hold totals for ages 1 to 15
        // Index 0 will be unused to maintain 1-based indexing for age
        long long counts[16] = {0};
        double totalRatings[16] = {0};
        double totalAbsRatings[16] = {0};

        THash hX, hO;
        TByte dummyZero, age, x3, x4, o3, o4;
        TRating rating;

        // Read records in the order: hashCodeX, hashCodeO, 0, age, x3, x4, o3, o4, rating
        while (inFile.read(reinterpret_cast<char*>(&hX), sizeof(hX))) {
            inFile.read(reinterpret_cast<char*>(&hO), sizeof(hO));
            inFile.read(reinterpret_cast<char*>(&dummyZero), sizeof(dummyZero));
            inFile.read(reinterpret_cast<char*>(&age), sizeof(age));
            inFile.read(reinterpret_cast<char*>(&x3), sizeof(x3));
            inFile.read(reinterpret_cast<char*>(&x4), sizeof(x4));
            inFile.read(reinterpret_cast<char*>(&o3), sizeof(o3));
            inFile.read(reinterpret_cast<char*>(&o4), sizeof(o4));
            inFile.read(reinterpret_cast<char*>(&rating), sizeof(rating));

            // Only track ages within the specified range (1 to 15)
            if (age >= 1 && age <= 15) {
                counts[age]++;
                totalRatings[age] += rating;
                totalAbsRatings[age] += std::abs(static_cast<double>(rating));
            }
        }

        inFile.close();

        // Print results for 2026 data analysis
        std::cout << std::left << std::setw(6) << "Age"
                  << std::setw(12) << "Count"
                  << std::setw(18) << "Avg Rating"
                  << "Avg Abs Rating" << std::endl;
        std::cout << std::string(55, '-') << std::endl;

        for (int i = 1; i <= 15; ++i) {
            if (counts[i] > 0) {
                double avgRating = totalRatings[i] / counts[i];
                double avgAbsRating = totalAbsRatings[i] / counts[i];

                std::cout << std::left << std::setw(6) << i
                          << std::setw(12) << counts[i]
                          << std::setw(18) << std::fixed << std::setprecision(2) << avgRating
                          << avgAbsRating << std::endl;
            } else {
                std::cout << std::left << std::setw(6) << i
                          << "0 nodes found for this age." << std::endl;
            }
        }
    }
};

int main() {
    NodeStatisticsUtility util;
    util.processFile();
    return 0;
}
