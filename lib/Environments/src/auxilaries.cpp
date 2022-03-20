#include "auxilaries.h"

#include <iostream>
#include <limits>
#include <random>

static std::random_device rd;
static std::mt19937 mt(rd());


double radians2degree(double radians)
{
  return radians*180/globals::pi;
}

double degree2radians(double degree)
{
  return degree*globals::pi/180;
}

double random_double(double min, double max)
{
  std::uniform_real_distribution<> dist(min, max);
  return dist(mt);
}

int random_sgn()
{
  return sgn(random_double(-1,1));
}

void rand_shuffle_range(int range, std::vector<int> *result)
{
  for(int x=0; x < range; x++)
  {
    result->push_back(x);
  }
  std::shuffle(result->begin(), result->end(), mt);
}

int rand_int(int min, int max)
{
  std::uniform_int_distribution<> dist(min, max);
  return dist(mt);
}



void ignore_line(std::istream &is)
{
  is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
