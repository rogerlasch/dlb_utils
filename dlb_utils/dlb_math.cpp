

#include<cmath>
#include<chrono>
#include<random>
#include"dlb_types.h"
#include"dlb_math.h"

#pragma warning(disable: 4244)

namespace dlb
{
int32 dlb_random_int32(int32 min, int32 max)
{
  unsigned seed =static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
std::default_random_engine generator (seed);
std::uniform_int_distribution<long> distribution(min, max);
return distribution(generator);
}

int64 dlb_random_int64(int64 min, int64 max)
{
  unsigned seed =static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
std::default_random_engine generator (seed);
std::uniform_int_distribution<int64> distribution(min, max);
return distribution(generator);
}

double dlb_round(double number, uint32 decimal_places)
{
if(decimal_places>15)
{
decimal_places=15;
}
int64 intpart=static_cast<int64>(number);
double decimal=number-static_cast<double>(intpart);
uint64 places=pow(10, decimal_places);
decimal=double(int64(decimal*places+0.5))/places;
double final=static_cast<double>(intpart)+decimal;
return final;
}

bool dlb_getlucky(int32 percentage)
{
int32 number=dlb_random_int32(0, 100);
return number<=percentage;
}
}
