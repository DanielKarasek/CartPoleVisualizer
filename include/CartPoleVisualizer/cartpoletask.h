#ifndef CARTPOLETASK_H
#define CARTPOLETASK_H

#include "../../lib/neat/include/neat/task.h"
#include "../../lib/Environments/include/Environments/CartPole/CartPole.h"
#include "neat/neuralnet.h"

#include <memory>

class CartPoleTask : public Task
{
public:
  CartPoleTask();

  virtual int get_input_shape() override;
  virtual int get_output_shape() override;
  virtual void evaluate(std::shared_ptr<NN> neural_net, TaskEvaluationResult *out, bool verbose = false) override;
protected:
  double single_eval(std::shared_ptr<NN> neural_net);
  CartPole m_cart_pole;


};
#endif // CARTPOLETASK_H
