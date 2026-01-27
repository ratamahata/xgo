//---------------------------------------------------------------------------


#pragma hdrstop

#include "Logger.h"
#include <iostream>

//---------------------------------------------------------------------------

#pragma package(smart_init)

Logger::Logger() {
        hitCount = 0;
        missAgeCount = 0;
        missHashCount = 0;
        missIndexCount = 0;
        lastError = NULL;
        expandEven = 0;
        expandOdd = 0;

        parents1Culled1 = 0;
        parents1Culled2 = 0;
        parents2Culled1 = 0;
        parents2Culled2 = 0;
        parents3Culled1 = 0;
        parents3Culled2 = 0;
        parents4Culled1 = 0;
        parents4Culled2 = 0;
        parents5Culled1 = 0;
        parents5Culled2 = 0;
}

void Logger::hit() {
        ++hitCount;
}

void Logger::missAge() {
        ++missAgeCount;
}

void Logger::missHash() {
        ++missHashCount;
}

void Logger::missIndex() {
        ++missIndexCount;
}

void Logger::error(const char* message) {
        this->lastError = message;
}

void Logger::log(const char* message) {
        std::cout << message << std::endl;
}

void Logger::expand(int count) {
        if (count%2) {
                ++expandOdd;
        } else {
                ++expandEven;
        }
}

void Logger::printLastError(char *buffer) {

        //sprintf(buffer, "%d ", (expandEven*100 / (1+expandOdd + expandEven)));

        if (lastError == NULL) {
        sprintf(buffer, "%d/%d/%d/%d/%d - %d/%d/%d/%d/%d",
                parents1Culled1, parents2Culled1, parents3Culled1, parents4Culled1, parents5Culled1,
                parents1Culled2, parents2Culled2, parents3Culled2, parents4Culled2, parents5Culled2
        );
        } else {

        sprintf(buffer,
                lastError != NULL
                        ? lastError : missAgeCount > 0
                        ? "Hash collisions %d"
                        : " ", missAgeCount);
        }
}
