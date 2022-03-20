#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "../../lib/Environments/include/Environments/CartPole/CartPole.h"
#include "cartpolerenderer.h"
#include "solverinterface.h"

#include <map>

#include <QObject>
#include <QTimer>
#include <QKeyEvent>

class Mediator : public QObject
{
  Q_OBJECT
public:
  explicit Mediator(QObject *parent = nullptr);
  void attach_renderer(CartPoleRenderer *renderer);
  void attach_solver(const std::string &name,std::shared_ptr<SolverInterface> solver = nullptr){m_solver_map.insert({name, solver});}

public slots:

  void step();
  void get_force();

  void reset();
  void set_speed(double new_game_speed);
  void set_solver(const std::string &name){m_solver = m_solver_map[name];};
  void toggle_pause();

protected:


  CartPole m_cart;
  CartPoleTransition m_current_transition;

  std::shared_ptr<SolverInterface> m_solver{nullptr};

  std::map<std::string, std::shared_ptr<SolverInterface>> m_solver_map{};

  double m_next_get_force{0.02};
  double m_fps_inverted{1.0/100.0};
  double m_game_speed{1};

  CartPole::ForceDirection m_force{CartPole::none};
  QTimer *m_timer;

  bool m_paused{false};

  CartPoleRenderer *m_renderer;
signals:

};

#endif // MEDIATOR_H
