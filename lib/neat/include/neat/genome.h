#ifndef GENOME_H
#define GENOME_H

#include "neuralnet.h"
#include <memory>

struct MutateArgs
{
  double new_connection_prob;
  double split_connection_prob;
  double permute_connections_prob;
  double random_weight_percentage;
  MutateArgs(double new_connection_prob, double split_connection_prob,
             double permute_connections_prob, double random_weight_percentage):
    new_connection_prob{new_connection_prob},
    split_connection_prob{split_connection_prob},
    permute_connections_prob{permute_connections_prob},
    random_weight_percentage{random_weight_percentage}
  {}
};


class Genome
{
public:
  Genome(std::shared_ptr<NN> nn): m_nn{nn}{};

  void mutate();

  std::shared_ptr<NN> get_nn()const{return m_nn;}

  double get_fitness()const{return m_fitness;}
  bool is_solved()const{return m_solved;}

  void set_fitness(double fitness){m_fitness = fitness;}
  void set_solved(bool solved){m_solved = solved;}


  static void set_innovation_number(int innov_number){S_INNOVATION_NUMBER = innov_number;}

  static void set_new_connection_prob(double new_connection_prob){S_MUTATE_ARGS.new_connection_prob = new_connection_prob;}
  static void set_split_connection_prob(double split_connection_prob){S_MUTATE_ARGS.split_connection_prob = split_connection_prob;}
  static void set_permute_connection_prob(double permute_connection_prob){S_MUTATE_ARGS.permute_connections_prob = permute_connection_prob;}
  static void set_random_weight_percentage(double random_weight_percentage){S_MUTATE_ARGS.random_weight_percentage = random_weight_percentage;}


  friend std::shared_ptr<Genome> cross_genomes(std::shared_ptr<Genome> parent1, std::shared_ptr<Genome> parent2, double percentage_disable);

protected:
  static int S_INNOVATION_NUMBER;
  static MutateArgs S_MUTATE_ARGS;
  std::shared_ptr<NN> m_nn;

  double m_fitness{};
  bool m_solved{};


};



#endif // GENOME_H
