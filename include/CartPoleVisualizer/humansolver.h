#ifndef HUMANSOLVER_H
#define HUMANSOLVER_H

#include "solverinterface.h"
#include "../../lib/Environments/include/Environments/CartPole/CartPole.h"
#include <QObject>
#include <QKeyEvent>

class HumanSolver : public QObject, public SolverInterface
{
  Q_OBJECT
public:
  explicit HumanSolver(QObject *parent = nullptr):QObject{parent}, SolverInterface{}{}

  void get_outputs(std::vector<double> inputs, std::vector<double> *out) override;
  void reset_solver() override;

public slots:
  void set_force(QKeyEvent *event);

signals:

protected:
  CartPole::ForceDirection m_force{CartPole::none};

};

#endif // HUMANSOLVER_H
