#ifndef TASK_H
#define TASK_H

#include <memory>
#include "neuralnet.h"

struct TaskEvaluationResult
{
  double total_reward;
  bool solved;
};

class Task
{
public:
  Task();
  virtual int get_input_shape() = 0;
  virtual int get_output_shape() = 0;
  virtual void evaluate(std::shared_ptr<NN> neural_net, TaskEvaluationResult *out, bool verbose = false) = 0;

};

#endif // TASK_H
