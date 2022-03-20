#include "neat.h"
#include "task.h"

#include <iostream>

NEAT::NEAT(std::shared_ptr<Task> task):
m_task{task}
{
  Genome::set_innovation_number((m_task->get_input_shape()+1) * m_task->get_output_shape());
}

std::shared_ptr<Genome> NEAT::epoch()
{
  double total_fit = std::accumulate(m_species.begin(), m_species.end(), 0, []
                                     (const double &acc, const auto &species)
                                     {return acc+species->get_average_fitness();});
  std::vector<std::shared_ptr<Genome>> genome_pool;

  std::cout << "generation number: "<< m_generetion_num << " starting\n" << std::flush;

  std::cout << "mating\n" << std::flush;
  for (auto &species : m_species)
  {
    int spawns_required = species->calculate_spawns_required(m_population_size, total_fit);
    for(int i=0; i<spawns_required; i++)
    {
      auto parent1 = species->tournament(m_tournament_percentage, m_tournament_k);
      auto parent2 = species->tournament(m_tournament_percentage, m_tournament_k);
      auto child = cross_genomes(parent1, parent2, m_percentage_disabled);
      child->mutate();
      genome_pool.push_back(child);
    }
  }

  std::cout << "filling extra\n" << std::flush;
  for (int i = genome_pool.size(); i < m_population_size; i++)
  {
    auto nn = std::make_shared<NN>(m_task->get_input_shape(), m_task->get_output_shape());
    auto gen = std::make_shared<Genome>(nn);
    genome_pool.push_back(gen);
  }

  std::cout << "adding to species\n" << std::flush;
  for (auto &gen : genome_pool)
  {
    bool found = false;
    for (auto &species : m_species)
    {
      double distance = species->distance_from_leader(gen);
      if (distance < m_delta_max)
      {
        species->add_genome(gen);
        found = true;
        break;
      }
    }
    if (!found)
    {
      auto new_species = std::make_shared<Species>(gen);
      m_species.push_back(new_species);
    }
  }

  std::cout << "Evaluation step over all species\n" << std::flush;
  std::shared_ptr<Genome> to_return_gen = nullptr;
  for (auto &species : m_species)
  {
    auto result_gen = species->step(m_task);
    if (result_gen)
      to_return_gen = result_gen;
  }

  std::cout << "species count: " << m_species.size() << '\n' << std::flush;
  m_generetion_num++;
  return to_return_gen;
}
