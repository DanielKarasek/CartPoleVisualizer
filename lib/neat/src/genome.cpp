#include "genome.h"
#include "auxilaries.h"
#include <iostream>

int Genome::S_INNOVATION_NUMBER = 0;
MutateArgs Genome::S_MUTATE_ARGS{5, 15, 90, 10};


void Genome::mutate()
{
  if (random_double(0, 100) < S_MUTATE_ARGS.new_connection_prob)
  {
    Connection con;
    m_nn->create_random_connection(S_INNOVATION_NUMBER, &con);
    S_INNOVATION_NUMBER++;
  }
  if (random_double(0, 100) < S_MUTATE_ARGS.split_connection_prob)
  {
    m_nn->split_random_connection(S_INNOVATION_NUMBER);
    S_INNOVATION_NUMBER += 2;
  }
  if (random_double(0, 100) < S_MUTATE_ARGS.permute_connections_prob)
  {
    m_nn->permute_connections(100 - S_MUTATE_ARGS.random_weight_percentage);
  }
}



std::shared_ptr<Genome> cross_genomes(std::shared_ptr<Genome> parent1, std::shared_ptr<Genome> parent2, double percentage_disable)
{
  auto new_nn = cross_nn(*parent1->get_nn(), *parent2->get_nn(), parent1->get_fitness() < parent2->get_fitness(), percentage_disable, false);
  return std::make_shared<Genome>(new_nn);
}
