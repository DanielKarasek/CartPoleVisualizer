#include "mainwindow.h"
#include "cartpolerenderer.h"
#include "mediator.h"
#include "auxilaries.h"
#include "humansolver.h"
#include "cartpoletask.h"

#include "nnsolver.h"
#include "../lib/neat/include/neat/neat.h"

#include "settingbar.h"

#include <iostream>
#include <QApplication>
#include <QHBoxLayout>
#include <memory>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
  //heating up random generator
  rand_int(10,20);
  rand_int(10,20);
  rand_int(10,20);
  rand_int(10,20);

  NEAT neat{std::make_shared<CartPoleTask>()};
  std::shared_ptr<Genome> genome;
  while(true)
  {
    genome = neat.epoch();
    if (genome)
    {
      break;
    }
  }

  CartPoleTask cart;
  TaskEvaluationResult res;
  cart.evaluate(genome->get_nn(), &res, true);
  std::shared_ptr<NNSolver> neat_solver = std::make_shared<NNSolver>(genome->get_nn());
  std::shared_ptr<HumanSolver> human_solver = std::make_shared<HumanSolver>();

  QApplication a(argc, argv);

  MainWindow w;
  QHBoxLayout *layout = new QHBoxLayout();

  Mediator mediator;
  CartPoleRenderer renderer;
  SettingBar bar;

  mediator.attach_renderer(&renderer);
  mediator.attach_solver("None", nullptr);
  mediator.attach_solver("Human", human_solver);
  mediator.attach_solver("Neat", neat_solver);

  bar.add_solver_value("None");
  bar.add_solver_value("Human");
  bar.add_solver_value("Neat");

  mediator.set_solver("None");

  renderer.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  bar.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  QObject::connect(&renderer, &CartPoleRenderer::key_event, &(*human_solver), &HumanSolver::set_force);
  QObject::connect(&bar, &SettingBar::speed_value_changed, &mediator, &Mediator::set_speed);
  QObject::connect(&bar, &SettingBar::pause_pushed, &mediator, &Mediator::toggle_pause);
  QObject::connect(&bar, &SettingBar::solver_combo_text_change, &mediator, &Mediator::set_solver);

  layout->addWidget(&renderer);
  layout->addWidget(&bar);

  w.centralWidget()->setLayout(layout);

  w.show();

  mediator.reset();

  return a.exec();
}
