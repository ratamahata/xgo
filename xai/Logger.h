//---------------------------------------------------------------------------

#ifndef LoggerH
#define LoggerH
#include <stdio.h>
//---------------------------------------------------------------------------
#include <TNode.h>
#include <Persister.h>

class Logger {

  private:
        int hitCount;
        int missAgeCount;
        int missHashCount;
        int missIndexCount;
        int missExpandCount;
        const char* lastError;
        int expandEven;
        int expandOdd;
        int reserved;

        Persister *persister;
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
        void missExpand(TNode *node);

        void cull(TRating ratingOld, TRating max_rating, TNode *node);
        void expand(int count);

        void error(const char *message);
        void log(const char *message);
        void log(const char *message, const int i);

        void printLastError(char *buffer);
};
#endif
