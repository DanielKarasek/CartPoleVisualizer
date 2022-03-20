#ifndef SOLVER_INTERFACE_H
#define SOLVER_INTERFACE_H

#include <vector>

class SolverInterface
{
public:
  SolverInterface(){};
  virtual void get_outputs(std::vector<double> inputs, std::vector<double> *out) = 0;
  virtual void reset_solver() = 0;
};

#endif // SOLVER_H
