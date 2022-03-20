#ifndef NNSOLVER_H
#define NNSOLVER_H

#include "solverinterface.h"
#include "neat/neuralnet.h"
#include <memory>

class NNSolver : public SolverInterface
{
public:
  NNSolver(std::shared_ptr<NN> nn):m_nn{nn}{};
  virtual void get_outputs(std::vector<double> inputs, std::vector<double> *out) override;
  virtual void reset_solver() override;


protected:
  std::shared_ptr<NN> m_nn;

};

#endif // NNSOLVER_H
