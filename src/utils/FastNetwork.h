#ifndef NEAT_FASTNETWORK_H
#define NEAT_FASTNETWORK_H

#include <vector>

#include "../neat/NetworkGenome.h"

/**
 * Neural network representation used for calculating output fast.
 *
 * Each node has an index. Input nodes have indices [0-input_count), hidden nodes [input_count, node_count-output_count),
 * and output nodes [node_count-output_count. node_count).
 * Input nodes and output nodes have the right order.
 * Nodes are assigned such indices that every next node is calculated using previous nodes.
 * (if node A leads to node B, index A is less than index B)
 */
class FastNetwork {
public:
    int node_count;
    int input_count;
    int output_count;

    /**
     * Array containing information about connections.
     * Vector at index i corresponds to connections leading into node with index i.
     * First element in pair is the index of the connected node, the second element is the weight.
     */
    std::vector<std::pair<int, double>> *connections;

    /**
     * Buffer for calculating node values.
     */
    double *values;

    /**
     * Create a FastNetwork from genes in genome.
     * @param genome
     */
    explicit FastNetwork(const NetworkGenome &genome);

    /**
     * Calculate values of nodes and return a pointer to output values.
     * @param inputs input node values
     * @return pointer to output values (points at some location in values array)
     */
    double * calculate(const double * inputs) const;

    /**
     * Activation function on each node.
     * @param x argument
     * @return value at x
     */
    static double activation(double x);

    FastNetwork &operator=(const FastNetwork& network);

    FastNetwork() = default;

    ~FastNetwork();
};


#endif
