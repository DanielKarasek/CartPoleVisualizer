#ifndef CARTPOLE_ASJFA_H
#define CARTPOLE_ASJFA_H

#include <iostream>

struct CartPoleState
{
  double x;
  double x_speed;
  double theta;
  double theta_speed;

  friend std::ostream &operator<<(std::ostream &os, const CartPoleState &state);
};

struct CartRenderConstraints
{
  double x_limit;
  double pole_lenght;
};

struct CartFunctionalConstraints
{
  double max_step_size;
  double x_limit;
  double theta_degree_limit;
};

struct CartPoleTransition
{
  const CartPoleState *state;
  double r;
  bool done;

  friend std::ostream &operator<<(std::ostream &os, const CartPoleTransition &transition);
};
// transition -> R, D

class CartPole
{
  public:
    CartPole(){}

    enum ForceDirection
    {
      left = -1,
      right = 1,
      none = 0
    };
    
    bool step(CartPole::ForceDirection direction, CartPoleTransition *state_out); // is clipped into direction -1 or 1
    void reset(CartPoleTransition *state_out);

    void get_state(CartPoleTransition *state){state->state = &m_cur_state;}
    void get_cart_render_constraints(CartRenderConstraints *out);

    void set_step_size(double ms);


  protected:
    void update_state(double force);
    
    CartPoleState m_cur_state;
    CartFunctionalConstraints m_functional_constraints{0.1, 2, 12};
    bool m_is_done{true};

    double m_gravity{9.8};// pos constant
    double m_force{3.0}; //base magnitute of force applied
    double m_pole_mass{0.1};
    double m_cart_mass{1.};
    double m_pole_half_length{0.5};
    double m_center_mass_pole{m_pole_mass*m_pole_half_length};//
    double m_total_mass{m_pole_mass+m_cart_mass};
    double m_step_size{0.02};
};


#endif
