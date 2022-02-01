#include <stack>
#include <iostream>
#include "FastNetwork.h"
#include "GraphNetwork.h"

FastNetwork::FastNetwork(const NetworkGenome &genome) : input_count(genome.input_count),
                                                        output_count(genome.output_count),
                                                        node_count(genome.node_count()),
                                                        values(new double[node_count]) {
    // Calculate the order in which nodes can be evaluated.
    GraphNetwork graph(genome);
    int *order = graph.evaluation_order();

    // Map from node number to node index.
    int * node_index = new int[node_count];

    // Map order to current indices, creating new indices.
    for (int i = 0; i < node_count; i++) {
        for(int j = 0; j < node_count; j++) {
            if(order[j] == i) {
                node_index[i] = j;
            }
        }
    }

    delete[] order;

    // Add each connection if its gene is enabled
    connections = new std::vector<std::pair<int, double>>[node_count];
    for (const auto &[innovation, g] : genome.genome) {
        if (!g.enabled) continue;

        connections[node_index[g.out]].emplace_back(node_index[g.in], g.weight);
    }

    delete [] node_index;
}

FastNetwork::~FastNetwork() {
    delete[] values;
    delete[] connections;
}

double *FastNetwork::calculate(const double *inputs) const {
    for(int i = 0; i < input_count; i++) {
        values[i] = inputs[i];
    }

    for(int i = input_count; i < node_count; i++) {
        values[i] = 0;
        for(const auto& [from, weight] : connections[i]) {
            values[i] += values[from] * weight;
        }
        values[i] = activation(values[i]);
    }

    return values + node_count - output_count;
}

double FastNetwork::activation(double x) {
    return 1.0 / (1 + std::exp(-4.9 * x));
}

FastNetwork &FastNetwork::operator=(const FastNetwork &network) {
    if(this == &network) return *this;

    node_count = network.node_count;
    input_count = network.input_count;
    output_count = network.output_count;
    delete[] values;
    values = new double[node_count];
    std::copy(network.values, network.values + node_count, values);
    delete[] connections;
    connections = new std::vector<std::pair<int, double>>[node_count];
    std::copy(network.connections, network.connections+ node_count, connections);

    return *this;
}

