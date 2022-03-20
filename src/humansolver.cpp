#include "humansolver.h"
#include "solverinterface.h"

void HumanSolver::get_outputs(std::vector<double> inputs, std::vector<double> *out)
{
  for (int x{0}; x<4; ++x)
  {
    out->push_back(0);
  }
  out->at(static_cast<int>(m_force)+1)  = 1;
}

void HumanSolver::reset_solver(){}

void HumanSolver::set_force(QKeyEvent *event)
{

  if (event->type() == QKeyEvent::KeyPress)
  {

    if(event->key() == 16777234)
    {
      m_force = CartPole::left;
    }
    if(event->key() == 16777236)
    {
      m_force = CartPole::right;
    }
  }
  else if(event->type() == QKeyEvent::KeyRelease)
  {
    if(event->key() == 16777236 || event->key() == 16777234)
    {
      m_force = CartPole::none;
    }
  }

}
