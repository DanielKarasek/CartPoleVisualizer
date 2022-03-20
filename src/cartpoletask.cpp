#include "cartpoletask.h"
#include "neat/neuralnet.h"
#include "Environments/CartPole/CartPole.h"

#include <memory>
#include <algorithm>
#include <vector>

CartPoleTask::CartPoleTask()
{

}

int CartPoleTask::get_input_shape()
{
  return 4;
}

int CartPoleTask::get_output_shape()
{
  return 3;
}

void CartPoleTask::evaluate(std::shared_ptr<NN> neural_net, TaskEvaluationResult *out, bool verbose)
{
  out->total_reward = 0;
  out->solved = false;
  for(int i{0}; i<3; ++i)
  {
    out->total_reward += single_eval(neural_net);
  }
  out->total_reward /= 3;
  if (out->total_reward >= 999.9)
  {
    out->solved = true;
  }
}



double CartPoleTask::single_eval(std::shared_ptr<NN> neural_net)
{
  CartPoleTransition state;
  m_cart_pole.reset(&state);

  std::vector<double> results;
  neural_net->nn_init();

  CartPole::ForceDirection dir;

  double total_reward {0};
  while(!state.done)
  {
    total_reward += state.r;
    neural_net->nn_run({state.state->theta,
                        state.state->theta_speed,
                        state.state->x,
                        state.state->x_speed},
                       &results);
    dir = static_cast<CartPole::ForceDirection>(std::max_element(results.begin(), results.end()) - results.begin() - 1);
    m_cart_pole.step(dir, &state);

    if (total_reward >= 1000)
    {
      return total_reward;
    }
  }
  return total_reward;
}
