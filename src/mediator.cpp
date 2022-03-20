#include "mediator.h"
#include "cartpolerenderer.h"

#include <QKeyEvent>
#include <QEvent>
#include <QObject>
#include <iostream>

#include <chrono>

Mediator::Mediator(QObject *parent)
  : QObject{parent}
{
  m_timer = new QTimer();
  QObject::connect(m_timer, &QTimer::timeout, this, &Mediator::step);
}

//functions
// podle klavesnice -> set_force, posledni smer a jestli byla posledni pustena z toho smeru
// step + render -> dokud neni fail -> restart (mozna pak restart klavesnici)
//slots
//set_force (int);
//step() -> zavola jednou update s m_force
//render(const *state) -> vezme m_state ->
//set_time_step(double ms)
//reset()
//render END, start

void Mediator::attach_renderer(CartPoleRenderer *renderer)
{
  CartRenderConstraints tmp;
  m_renderer = renderer;
  m_cart.get_cart_render_constraints(&tmp);
  renderer->set_constraints(tmp);
}


void Mediator::step()
{

  //step
  // tmp_t = current_time
  // render
  // if (next get_force < m_fps_inverted)
  // {if (m_fps_inverted < eps){get_force .. step to next .. plan next step .. plan next_get force}
  // else {env step -> get_force -> step to next -> plan next step .. plan next_get force}}
  // step to next
  // plan next
  // pridat do reset prvni ziskani force
  auto start_time = std::chrono::system_clock::now();
  CartPoleTransition state;
  if (m_next_get_force - 1e-4 < m_fps_inverted)
  {
    m_cart.set_step_size(m_next_get_force);
    m_cart.step(m_force, &state);
    get_force();
    double step_left = m_fps_inverted-m_next_get_force;
    if (step_left > 1.0e-4) // pokud nejsou dost blizko
    {
      m_cart.set_step_size(step_left);
      m_cart.step(m_force, &state);
      m_next_get_force = 0.02-step_left;
    }
    else
    {
      m_next_get_force = 0.02;
    }

    m_cart.set_step_size(m_fps_inverted);
  }
  else
  {
    m_cart.step(m_force, &state);
    m_next_get_force -= m_fps_inverted;
  }
  m_renderer->render(state.state);

  if (state.done)
  {
    m_timer->stop();
    reset();
  }
  auto end_time = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = end_time - start_time;
  m_timer->stop();
  if (!m_paused)
    m_timer->start(2000*m_fps_inverted/m_game_speed - elapsed.count());

}

void Mediator::get_force()
{
  CartPoleTransition state;
  if (m_solver)
  {
    m_cart.get_state(&state);
    std::vector<double> results;
    m_solver->get_outputs({state.state->theta,
                           state.state->theta_speed,
                           state.state->x,
                           state.state->x_speed},
                           &results);
    auto force = static_cast<CartPole::ForceDirection>(std::max_element(results.begin(), results.end()) - results.begin() - 1);
    m_force = force;
  }
  else
    m_force = CartPole::none;
}

void Mediator::reset()
{
  CartPoleTransition state;
  m_cart.reset(&state);

  if (m_solver)
  {
    m_solver->reset_solver();
  }
  m_renderer->render(state.state);
  m_timer->start(2000*m_fps_inverted/m_game_speed);

}

void Mediator::set_speed(double new_game_speed)
{
  m_game_speed = new_game_speed;
}

void Mediator::toggle_pause()
{
  m_paused = !m_paused;
  if(!m_paused)
  {
    step();
  }
}
