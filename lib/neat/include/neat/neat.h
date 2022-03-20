#ifndef NEAT_H
#define NEAT_H

#include "genome.h"
#include "species.h"
#include "task.h"

#include <memory>
#include <numeric> // std accumulate


class NEAT
{
public:
  NEAT(std::shared_ptr<Task> task);
  std::shared_ptr<Genome> epoch();

protected:

  std::shared_ptr<Task> m_task;
  std::vector<std::shared_ptr<Species>> m_species;

  bool m_elitism{true};

  double m_tournament_percentage{70};
  int m_tournament_k{50};

  int m_max_no_improvement{10};
  int m_population_size{150};

  double m_percentage_disabled{25};

  double m_delta_max{3};


  // stats
  int m_generetion_num{0};
};

#endif // NEAT_H
