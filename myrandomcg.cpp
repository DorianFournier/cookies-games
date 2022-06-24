#include "myrandomcg.h"
#include <QDateTime>

MyRandomCG::MyRandomCG(int seed)
{
    srand(seed);
}

int MyRandomCG::get(int max)
{
    return rand()%max+1;
}
