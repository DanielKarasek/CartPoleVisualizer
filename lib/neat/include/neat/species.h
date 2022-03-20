#ifndef SPECIES_H
#define SPECIES_H

#include "task.h"
#include "genome.h"
#include "neuralnet.h"

#include <vector>
#include <memory>

class Species
{
public:
  Species();
  Species(std::shared_ptr<Genome> gen);

  int calculate_spawns_required(int total_pop, double total_fitness);

  void add_genome(std::shared_ptr<Genome> genome);
  std::shared_ptr<Genome> step(std::shared_ptr<Task> task);

  std::shared_ptr<Genome> tournament(double percentage_in_tournament, int k);

  double distance_from_leader(std::shared_ptr<Genome> gen);

  double get_average_fitness(){return m_avg_fitness;}
  double get_best_fitness(){return m_best_fitness;}
  int get_last_improvment(){return m_last_improvement;}
  int get_age(){return m_age;}

  bool is_solved(){return m_solved;}
  std::shared_ptr<Genome> get_solved();


  static void set_dissjoint_coeff(double disjoint_coeff){S_DIST_COEFFICIENTS.disjoint_coeff = disjoint_coeff;}
  static void set_excess_coeff(double excess_coeff){S_DIST_COEFFICIENTS.excess_coeff = excess_coeff;}
  static void set_weight_coeff(double weight_coeff){S_DIST_COEFFICIENTS.weight_diff_coef = weight_coeff;}

protected:
  static NNDistCoeffs S_DIST_COEFFICIENTS;

  std::vector<std::shared_ptr<Genome>> m_individuals;

  int m_age{0};
  int m_last_improvement{0};

  double m_avg_fitness{0};
  double m_best_fitness{0};

  bool m_solved{false};

  std::shared_ptr<Genome> m_leader;
};

#endif // SPECIES_H
