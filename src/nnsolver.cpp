#include "nnsolver.h"
#include "neat/neuralnet.h"
#include <iostream>
#include <memory>

void NNSolver::get_outputs(std::vector<double> inputs, std::vector<double> *out)
{
  m_nn->nn_run(inputs, out);
}


void NNSolver::reset_solver()
{
  m_nn->nn_init();
}
