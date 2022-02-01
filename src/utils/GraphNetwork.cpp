#include <queue>
#include <algorithm>
#include <stack>
#include <iostream>

#include "GraphNetwork.h"

bool GraphNetwork::connection_exists(int a, int b) const {
    return std::any_of(connections.at(b).begin(), connections.at(b).end(), [a](auto p) { return p.first == a; });
}

GraphNetwork::GraphNetwork(const NetworkGenome &genome) {
    // add input nodes to input set
    for (int i = 0; i < genome.input_count; i++) {
        input.insert(i);
    }

    // add output nodes to output set
    for (int i = genome.input_count; i < genome.input_count + genome.output_count; i++) {
        output.insert(i);
    }

    // Construct connections and add nodes to the set
    for (const auto &p : genome.genome) {
        const Gene &gene = p.second;
        nodes.insert(gene.in);
        nodes.insert(gene.out);

        if (!connections.contains(gene.in)) connections[gene.in] = std::vector<std::pair<int, double>>();
        if (!connections.contains(gene.out)) connections[gene.out] = std::vector<std::pair<int, double>>();

        connections[gene.out].emplace_back(gene.in, gene.enabled ? gene.weight : INFINITY);
    }
}

void GraphNetwork::mark_evaluation_order(int node, std::vector<int> &order) const {
    if(std::find(order.begin(), order.end(), node) != order.end()) {
        return;
    }

    for(const auto& [from, weight]: connections.at(node)) {
        mark_evaluation_order(from, order);
    }

    if(!output.contains(node)) order.push_back(node);
}

int *GraphNetwork::evaluation_order() const {
    std::vector<int> order;
    for(int node : output) {
        mark_evaluation_order(node, order);
    }
    for(int node : output) {
        order.push_back(node);
    }

    int* order_array = new int[nodes.size()];
    for(int i = 0; i < nodes.size(); i++) {
        order_array[i] = order[i];
    }

    return order_array;
}


std::pair<int, int> GraphNetwork::get_new_random_connection(std::default_random_engine &engine) const {
    // Set with all nodes from which connection can be created
    auto nodes_possible = nodes;
    erase_if(nodes_possible, [this](const auto &node) { return output.contains(node); });

    // Do until correct connection found
    while (!nodes_possible.empty()) {
        // Find from node
        int from = random_from_set(nodes_possible, engine);

        auto connection = get_new_random_connection_from(from, engine);

        if (connection != std::pair<int, int>()) return connection;

        nodes_possible.erase(from);
    }

    return {};
}

std::pair<int, int> GraphNetwork::get_new_random_connection_from(int from, std::default_random_engine &engine) const {
    // Set with all nodes to which connection can be created
    std::set<int> nodes_possible = nodes;

    // Exclude this node
    nodes_possible.erase(from);

    // Exclude nodes leading to this from from set to avoid loops and exclude input nodes
    auto previous = get_previous_nodes(from);
    erase_if(nodes_possible,
             [this, previous](const auto &node) { return input.contains(node) || previous.contains(node); });

    // Do until correct connection found
    while (!nodes_possible.empty()) {
        // Find to from
        int to = random_from_set(nodes_possible, engine);
        if (!connection_exists(from, to)) return {from, to};

        nodes_possible.erase(to);
    }
    return {};
}

std::set<int> GraphNetwork::get_previous_nodes(int node) const {
    // Set empty if node is input node
    if (input.contains(node)) return {};

    // Set of previous nodes
    std::set<int> previous;

    // Go through each connection and add each neighbor's previous connections to set
    for (const auto &connection : connections.at(node)) {
        previous.insert(connection.first);
        previous.merge(get_previous_nodes(connection.first));
    }

    return previous;
}

int GraphNetwork::random_from_set(const std::set<int> &set, std::default_random_engine &engine) {
    // Get random index of node
    std::uniform_int_distribution<int> distribution(0, (int) set.size() - 1);
    int index = distribution(engine);

    // Get a node in sequence
    for (int node : set) {
        if (index == 0) return node;
        index--;
    }

    // Should never be reached
    return -1;
}

int GraphNetwork::max_node() const {
    return *std::max_element(nodes.begin(), nodes.end());
}

std::vector<std::vector<int>> GraphNetwork::get_layers() const {
    // Get an ordering where every next node is dependant only on the previous ones
    int *order = evaluation_order();

    // Layering with only one empty layer
    std::vector<std::vector<int>> layers({std::vector<int>()});

    // Go through nodes in order of evaluation
    for (int i = 0; i < nodes.size(); i++) {
        // Current node
        int node = order[i];

        // Nodes current node depends on
        auto previous = get_previous_nodes(node);

        // If current node depends on any node in last layer
        if (std::any_of(layers.back().begin(), layers.back().end(),
                        [previous](int n) { return previous.contains(n); })) {
            // Add new layer with current node
            layers.push_back(std::vector<int>({node}));
        } else {
            // Add current node to last layer
            layers.back().push_back(node);
        }
    }

    delete[] order;
    return layers;
}

