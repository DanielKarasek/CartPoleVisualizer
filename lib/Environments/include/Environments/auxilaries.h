#ifndef AUXILARIES_1213_H
#define AUXILARIES_1213_H

#include <iostream>
#include <functional>
#include <random>

namespace globals
{
  inline constexpr double pi = 3.145;
}

double radians2degree(double radians);
double degree2radians(double degree);

double random_double(double lower, double higher);
int rand_int(int min, int max);
void rand_shuffle_range(int range, std::vector<int> *result);

void ignore_line(std::istream &is);

int random_sgn();

template <typename T>
int sgn(T value)
{
  return (T(0) < value) - (value < T(0));
}

template <typename T>
T get_integral_value(std::string_view text, std::function<bool(T)> condition)
{
  T val;
  while (true)
  {
    std::cout << text;
    std::cin >> val;
    if (std::cin.fail())
    {
      std::cout << "Input failed due to invalid value\n";
      std::cin.clear();
      ignore_line(std::cin);
    }
    else if (!condition(val))
    {
      std::cout << "Input failed due to value not passing condition\n";
      ignore_line(std::cin);
    }
    else
    {
      ignore_line(std::cin);
      return val;
    }
  }
}


#endif
