#include "activationfunctions.h"

#include <functional>
#include <cmath>

double sigmoid(double input, double x_factor)
{
  return 1/(1+std::exp(-x_factor*input));
}

ActivationFunction sigmoid_factory(double x_factor)
{
  return [x_factor](double input){return sigmoid(input, x_factor);};
}
