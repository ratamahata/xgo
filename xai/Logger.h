//---------------------------------------------------------------------------

#ifndef LoggerH
#define LoggerH
#include <stdio.h>
//---------------------------------------------------------------------------

class Logger {

  private:
        int hitCount;
        int missAgeCount;
        int missHashCount;
        int missIndexCount;
        const char* lastError;
        int expandEven;
        int expandOdd;
        int reserved;

  public:

        int parents1Culled1;
        int parents1Culled2;
        int parents2Culled1;
        int parents2Culled2;
        int parents3Culled1;
        int parents3Culled2;
        int parents4Culled1;
        int parents4Culled2;
        int parents5Culled1;
        int parents5Culled2;

        Logger();
        void hit();
        void missAge();
        void missHash();
        void missIndex();
        void expand(int count);

        void error(const char *message);

        void printLastError(char *buffer);
};
#endif
