#include "neuralnet.h"
#include "activationfunctions.h"
#include "auxilaries.h"

#include <iterator>
#include <list>
#include <iostream>
#include <vector>
#include <memory>


NN::NN(int input_shape, int output_shape)
:m_input_shape{input_shape}, m_output_shape{output_shape}
{
  // + 1 == implicit bias
  for (int i=0; i<m_input_shape+1; ++i)
  {
    m_nodes.push_back({NodeType::Sensor, [](double a){return a;}, 0, 0, 0});
  }
  for (int j=0; j<m_output_shape; ++j)
  {
    m_nodes.push_back({NodeType::Output, sigmoid_factory(m_magic_sigmoid_factor), 0, 0, 0});
  }
  int innovation_number = 0;
  for (int i=0; i<m_input_shape+1; ++i)
  {
    for (int j=0; j<m_output_shape; ++j)
    {
      //maybe randomize this
      add_connection(i, m_input_shape+1+j, innovation_number++, random_double(m_magic_sigmoid_60, -m_magic_sigmoid_60));
    }
  }
  auto iter {m_nodes.begin()};
  std::advance(iter, m_input_shape);
  iter->output = 1;
  iter->recursive_out = 1;
  iter->calculated = true;
}



double NN::get_node_out(int node_idx)
{
  auto iter_result_node = m_nodes.begin();
  std::advance(iter_result_node, node_idx);

  if (iter_result_node->calculated)
    return iter_result_node->output;

  double input{0};
  double add_val{0};

  if (m_verbose) std::cout << "Calculating node " << node_idx << '\n';

  for(const auto &connection : m_connections)
  {

    auto node2add_iter = m_nodes.begin();
    if (connection.out_idx == node_idx && connection.enabled)
    {
      std::advance(node2add_iter, connection.inp_idx);

      if (connection.is_recursive) add_val = node2add_iter->recursive_out * connection.weight;
      else add_val = get_node_out(connection.inp_idx) * connection.weight;

      input += add_val;

      if (m_verbose) std::cout << "\tAdding value: " << add_val << " from input node: " << connection.inp_idx << '\n';
    }
  }

  iter_result_node->output = iter_result_node->activation_fn(input);
  if (m_verbose)
  {
    std::cout << "\tRESULTING IN: " << input << " which equals to "
              << iter_result_node->output << " after applying activation function\n" << std::flush;
  }
  iter_result_node->calculated = true;
  return iter_result_node->output;
}


void NN::nn_run(std::vector<double> inputs, std::vector<double> *result)
{
  //setup first layer
  //obalit list aby se dalo snaz pristupovat
  result->clear();

  if (static_cast<int>(inputs.size()) < m_input_shape)
  {
    std::cout << "ERROR INPUT SHAPE, DIFFERENT THAN EXPECTED\n"
              << "\texpected shape " << m_input_shape << " got " << inputs.size() << std::flush;
    return;
  }
  // double get_node_out(node_idx);


  auto neuron_iter = m_nodes.begin();
  for (int i=0; i<m_input_shape; ++i)
  {
    neuron_iter->output = inputs[i];
    neuron_iter->calculated = true;
    ++neuron_iter;
  }
  // once more for bias
  neuron_iter->calculated = true;

  // for neuron in output neurons get value -> append
  for (int i=m_input_shape+1; i<m_output_shape+m_input_shape+1; i++)
  {
    result->push_back(get_node_out(i));
  }
  for (auto &node : m_nodes)
  {
    node.calculated = false;
    node.recursive_out = node.output;
  }
}

void NN::nn_init()
{
   for(auto &node: m_nodes)
   {
     node.recursive_out = 0;
     node.output = 0;
     node.calculated = false;
   }
   auto iter = m_nodes.begin();
   std::advance(iter, m_input_shape);
   iter->recursive_out = 1;
   iter->output = 1;
}

void NN::create_random_connection(int innovation_number, Connection *out)
{
  int inp_idx = rand_int(0, static_cast<int>(m_nodes.size()-1));
  int out_idx = rand_int(m_input_shape+1, static_cast<int>(m_nodes.size()-1));
  add_connection(inp_idx, out_idx, innovation_number, random_double(m_magic_sigmoid_60, -m_magic_sigmoid_60));
  out->inp_idx = inp_idx;
  out->out_idx = out_idx;

  if (m_verbose) std::cout << "Adding connection from " << inp_idx << " to " << out_idx << '\n';
}

// returns innovation number of split connection
int NN::split_random_connection(int innovation_number)
{
  auto iter = m_connections.begin();

  while (true)
  {
  int connection2split = rand_int(0, static_cast<int>(m_connections.size())-1);
  std::advance(iter, connection2split);
  if (iter->enabled && !iter->is_recursive)
    break;
  iter = m_connections.begin();
  }
  m_nodes.push_back({NodeType::Hidden, sigmoid_factory(m_magic_sigmoid_factor), 0, 0, 0});

  if (m_verbose) std::cout << "Splitting connection from: " << iter->inp_idx << " to: " << iter->out_idx << '\n';
  add_connection(iter->inp_idx, m_nodes.size()-1, innovation_number, 1);
  add_connection(m_nodes.size()-1, iter->out_idx, innovation_number+1, iter->weight);
  iter->enabled = false;
  return 0;
}

void NN::permute_connections(double permute_percentage)
{
  //
  for (auto &connection : m_connections)
  {
    if (random_double(0,100) < permute_percentage)
      connection.weight += m_magic_sigmoid_60*random_double(-m_magic_sigmoid_60/4, m_magic_sigmoid_60/4)/100;
    else
      connection.weight = random_double(m_magic_sigmoid_60, -m_magic_sigmoid_60);
  }
}

void NN::add_connection(int inp_idx, int out_idx, int innovation_number, double weight)
{
  bool is_recursive {false};
  if (is_recursive_connection(inp_idx, out_idx))
  {
    is_recursive = true;

    if(m_verbose) std::cout << "recursive connection from: " << inp_idx << " to " << out_idx << '\n';
  }
//  auto iter=m_connections.begin();
//  while (iter->out_idx < out_idx && iter != m_connections.end())iter++;
  m_connections.push_back({inp_idx, out_idx, weight, true, is_recursive, innovation_number});
}

bool in_int_vector(int value, const std::vector<int> &vec)
{
  for (auto val:vec)
  {
    if (value == val)
      return true;
  }
  return false;
}

bool NN::is_recursive_connection(int inp_idx, int out_idx)
{
  //(inp_idx) ---weight--> (out_idx) kam se toto dostane vsude->
  std::vector<int> checked; // idx of input nodes to develop
  std::vector<int> to_check;
  if (inp_idx == out_idx) return true;
  to_check.push_back(out_idx);
  do
  {
    int checked_input_node = to_check[to_check.size()-1];
    to_check.pop_back();
    for (const auto &connection : m_connections)
    {
      if (checked_input_node == connection.inp_idx)
      {
        // pokud out idx = inp idx ze vstupu return true
        if (connection.out_idx == inp_idx) return true;
        // out idx neni v checked ani to_check -> pridat do to check
        if (!in_int_vector(connection.out_idx, to_check) && !in_int_vector(connection.out_idx, checked))
        {
          to_check.push_back(connection.out_idx);
        }
      }
    }
    checked.push_back(checked_input_node);
  } while(!to_check.empty());
  return false;
}

std::shared_ptr<NN> cross_nn(const NN &first_nn, const NN &second_nn, int fitter_idx, double dissable_prob, bool verbose = false)
{
  auto out = std::make_shared<NN>(first_nn.m_input_shape, first_nn.m_output_shape);
  out->m_connections.clear();
  out->m_nodes.clear();
  out->m_input_shape = first_nn.m_input_shape;
  out->m_output_shape = first_nn.m_output_shape;
  const NN *fitter_nn = fitter_idx==0 ? &first_nn : &second_nn;
  const NN *unfitter_nn = fitter_idx==0 ? &second_nn : &first_nn;

  out->m_nodes = fitter_nn->m_nodes;
  // fitter_nn -> najit stejne u unfitter_nn, neni_li
  auto unfitter_connection_iter = unfitter_nn->m_connections.begin();
  for (const auto &connection : fitter_nn->m_connections)
  {
    while(unfitter_connection_iter->innovation_number < connection.innovation_number &&
          unfitter_connection_iter != unfitter_nn->m_connections.end())
    {
      ++unfitter_connection_iter;

    }
    if (unfitter_connection_iter != unfitter_nn->m_connections.end())
    {
      if (verbose)
        std::cout << unfitter_connection_iter->innovation_number << ' ' << connection.innovation_number << '\n';

      if (random_double(0,100) < 50) out->m_connections.push_back(connection);
      else out->m_connections.push_back(*(unfitter_connection_iter));

      if (random_double(0, 100) < dissable_prob && (!unfitter_connection_iter->enabled || !connection.enabled))out->m_connections.back().enabled = false;
      else out->m_connections.back().enabled = true;
    }
    else
    {
      out->m_connections.push_back(connection);
    }
  }
  return out;
}


double nn_distance(const NN &first_nn, const NN &second_nn, NNDistCoeffs &dist_coeffs, bool verbose = false)
{
  int disjoint = 0;
  int excess = 0;
  int matching_count = 0;
  double weight_diff = 0;
  double max_lenght =  first_nn.m_connections.size() > second_nn.m_connections.size() ? first_nn.m_connections.size() : second_nn.m_connections.size();
  // while ani jedno end split in 3 innov1 > innov2, disjoint ++inov1
  //                                innov2 < innov1, disjoint ++inov2
  //                                else += weight diff;
  auto first_iter = first_nn.m_connections.begin();
  auto second_iter = second_nn.m_connections.begin();
  auto first_iter_end = first_nn.m_connections.end();
  auto second_iter_end = second_nn.m_connections.end();
  while (first_iter != first_iter_end && second_iter != second_iter_end)
  {
    if (first_iter->innovation_number < second_iter->innovation_number)
    {
      if (verbose) std::cout << "first nn disjoint at innov number: " <<  first_iter->innovation_number << '\n';
      ++disjoint;
      ++first_iter;
    }
    else if(first_iter->innovation_number > second_iter->innovation_number)
    {
      if (verbose) std::cout << "second nn disjoint at innov number: " <<  second_iter->innovation_number << '\n';
      ++disjoint;
      ++second_iter;
    }
    else
    {
      if (verbose)
      {
        std::cout << "matching at innov number: " << first_iter->innovation_number
                  << " and weight diff: " << std::abs(first_iter->weight - second_iter->weight) << '\n';
      }
      weight_diff += std::abs(first_iter->weight - second_iter->weight);
      ++matching_count;
      ++second_iter;
      ++first_iter;
    }
  }
  while(first_iter != first_iter_end)
  {
    if (verbose) std::cout << "excess of first nn at innov number: " <<  first_iter->innovation_number << '\n';
    ++excess;
    ++first_iter;
  }
  while(second_iter != second_iter_end)
  {
    if (verbose) std::cout << "excess of second nn at innov number: " <<  second_iter->innovation_number << '\n';
    ++excess;
    ++second_iter;
  }
  return disjoint * dist_coeffs.disjoint_coeff/ max_lenght +
         excess*dist_coeffs.excess_coeff/max_lenght +
         weight_diff*dist_coeffs.weight_diff_coef/matching_count;
}
