#include "neat/species.h"

#include "neat/task.h"
#include "neat/genome.h"
#include "neat/neuralnet.h"
#include "auxilaries.h"

#include <vector>
#include <memory>
#include <vector>


NNDistCoeffs Species::S_DIST_COEFFICIENTS = {1, 1, 2};


Species::Species()
{

}


Species::Species(std::shared_ptr<Genome> gen)
{
  m_leader = gen;
  add_genome(gen);
}

int Species::calculate_spawns_required(int total_pop, double total_fitness)
{
  return std::floor(total_pop*m_avg_fitness/total_fitness);
}


std::shared_ptr<Genome> Species::get_solved()
{
  for (auto &gen : m_individuals)
  {
    if (gen->is_solved()) return gen;
  }
  return std::make_shared<Genome>(nullptr);
}

std::shared_ptr<Genome> Species::step(std::shared_ptr<Task> task)
{

  std::shared_ptr<Genome> to_return = nullptr;

  double  fitness_sum{0};

  for (auto &indiv : m_individuals)
  {


    TaskEvaluationResult result;
    task->evaluate(indiv->get_nn(), &result);

    //indiv_fitness /= m_individuals.size();

    indiv->set_fitness(result.total_reward);
    indiv->set_solved(result.solved);
    if (result.solved)
    {
      m_solved = true;
      to_return = indiv;
    }
    fitness_sum += result.total_reward;
  }



  std::sort(m_individuals.begin(), m_individuals.end(),
            [](std::shared_ptr<Genome> &first, std::shared_ptr<Genome> &sec)
              {return first->get_fitness() > sec->get_fitness();});
  m_avg_fitness = fitness_sum/m_individuals.size();


  if (m_individuals[0]->get_fitness() > m_best_fitness)
  {
    m_last_improvement = 0;
    m_best_fitness = m_individuals[0]->get_fitness();
    m_leader = m_individuals[0];
  }
  m_age++;
  m_last_improvement++;

  return to_return;
}

void Species::add_genome(std::shared_ptr<Genome> genome)
{
  m_individuals.push_back(genome);
}

double Species::distance_from_leader(std::shared_ptr<Genome> gen)
{
  return nn_distance(*m_leader->get_nn(), *gen->get_nn(), S_DIST_COEFFICIENTS, false);
}

std::shared_ptr<Genome> Species::tournament(double percentage_in_tournament, int k)
{
  int choose_count = std::ceil(m_individuals.size()*percentage_in_tournament/100);
  k = std::min(choose_count, k);
  std::vector<int> random_vec;

  // zamicham prvnich n - ostatni ignoruju
  rand_shuffle_range(choose_count, &random_vec);
  double best_fitness = 0;
  std::shared_ptr<Genome> best_indiv;

  // vytahnu random k, ze zamichanych n a z nich ten nej
  for (int k_counter = 0; k_counter < k; k_counter++)
  {
    auto indiv = m_individuals[random_vec[k_counter]];
    if (indiv->get_fitness() > best_fitness)
    {
      best_fitness = indiv->get_fitness();
      best_indiv = indiv;
    }
  }
  return best_indiv;
}
