#include "CartPole/CartPole.h"
#include "auxilaries.h"

#include <cmath>
#include <iomanip>

bool CartPole::step(CartPole::ForceDirection direction, CartPoleTransition *state_out)
{
  if (m_is_done)
    return false;
  // upravit na silu

  double tmp_force = m_force * static_cast<double>(direction);
  update_state(tmp_force);
  state_out->r = 1.0;
  state_out->done = false;
  state_out->state = &m_cur_state;

  if (std::fabs(state_out->state->x) > m_functional_constraints.x_limit ||
      std::fabs(state_out->state->theta) > degree2radians(m_functional_constraints.theta_degree_limit))
  {
    m_is_done = true;
    state_out->done = true;
  }
  return true;
}

void CartPole::reset(CartPoleTransition *state_out)
{
  m_is_done = false;
  
  m_cur_state.x = 0;
  m_cur_state.x_speed = 0;
  m_cur_state.theta = degree2radians(random_double(1,3)) * random_sgn();
  m_cur_state.theta_speed = random_double(-0.007, 0.007);
  

  state_out->r = 0;
  state_out->done = false;
  state_out->state = &m_cur_state;
}

void CartPole::update_state(double force)
{

  double sin_theta = std::sin(m_cur_state.theta);
  double cos_theta = std::cos(m_cur_state.theta);
  double temp = force + m_center_mass_pole *
                      m_cur_state.theta_speed *
                      m_cur_state.theta_speed *
                      sin_theta;
  double theta_acc = m_gravity * sin_theta + cos_theta *(-temp/m_total_mass);
  theta_acc /= m_pole_half_length*(4/3-m_pole_mass*cos_theta*cos_theta/m_total_mass);
  double x_acc = (temp - theta_acc*cos_theta)/ m_total_mass;
  // O' = int(O''), O = int(O')
  // x' = int(x''), x = int(x')
  m_cur_state.theta = m_cur_state.theta + m_step_size * m_cur_state.theta_speed;
  m_cur_state.theta_speed = m_cur_state.theta_speed + m_step_size * theta_acc;
  m_cur_state.x = m_cur_state.x + m_step_size * m_cur_state.x_speed;
  m_cur_state.x_speed = m_cur_state.x_speed + m_step_size * x_acc;
}

void CartPole::get_cart_render_constraints(CartRenderConstraints *out)
{
  out->pole_lenght = m_pole_half_length * 2;
  out->x_limit = m_functional_constraints.x_limit;
}

void CartPole::set_step_size(double ms)
{
  if(m_step_size < m_functional_constraints.max_step_size) m_step_size = ms;
}


std::ostream &operator<<(std::ostream &os, const CartPoleState &state)
{
  std::cout << std::setprecision(9);
  std::cout << std::fixed;
  os << "Cartpole state:\n" << "x pos: " << state.x << "\tx speed: " << state.x_speed
     << "\ttheta: " << state.theta << "\ttheta speed:" << state.theta_speed << '\n';
  return os;
}
std::ostream &operator<<(std::ostream &os, const CartPoleTransition &transition)
{
  os << (*transition.state);
  return os;
}
