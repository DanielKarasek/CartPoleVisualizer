#ifndef ACTIVATIONFUNCTIONS_H
#define ACTIVATIONFUNCTIONS_H

#include <functional>

using ActivationFunction = std::function<double(double)>;

ActivationFunction sigmoid_factory(double x_factor);


#endif // ACTIVATIONFUNCTIONS_H
