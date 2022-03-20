#ifndef NEURALNET_H
#define NEURALNET_H

#include <functional>
#include <list>
#include <cmath>
#include <memory>


enum class NodeType
{
   Sensor,
   Hidden,
   Output
};

struct Node
{
  NodeType type;
  std::function<double(double)> activation_fn;
  double output;
  double recursive_out;
  bool calculated;
};

struct Connection
{
  int inp_idx;
  int out_idx;
  double weight;
  bool enabled;
  bool is_recursive;
  int innovation_number;
};

// pridat do distance funkce
struct NNDistCoeffs
{
  double disjoint_coeff;
  double excess_coeff;
  double weight_diff_coef;
};

// udelat tridu NEAT_POPULATION
// Bude mit funkci cross, pak interface na mutaci, jeste nevim jak presne
// kazdopadne pri topologicke inovaci musi vracet, pri rozpojeni, ktery connection rozpojen
// pri vzniku noveho spoje, mezi jakymi 2 prvky
//
class NN
{

public:
  NN(int input_shape, int output_shape);
  void nn_run(std::vector<double> inputs, std::vector<double> *result);
  void nn_init();

  void create_random_connection(int innovation_number, Connection *out);
  // returns innovation number of split connection
  int split_random_connection(int innovation_number);

  void permute_connections(double permute_percentage);

  void add_connection(int inp_idx, int out_idx, int innovation_number, double weight = 1);
  bool is_recursive_connection(int inp_idx, int out_idx);

  friend std::shared_ptr<NN> cross_nn(const NN &first_nn, const NN &second_nn, int fitter_idx, double dissable_prob, bool verbose);
  friend double nn_distance(const NN &first_nn, const NN &second_nn, NNDistCoeffs &dist_coeffs, bool verbose);

protected:

  double get_node_out(int node_idx);

  double m_magic_sigmoid_factor{4.7};
  // 1/(1+e^-kx) = a
  // a(1+e^-kx) = 1
  // log(1-a/a) = -kx

  double m_magic_sigmoid_60{-std::log((1-0.15)/0.15)/m_magic_sigmoid_factor};

  int m_input_shape{};
  int m_output_shape{};

  bool m_verbose{false};

  std::list<Node> m_nodes;
  std::list<Connection> m_connections;
};


#endif // NEURALNET_H
