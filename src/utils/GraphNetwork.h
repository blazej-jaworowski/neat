#ifndef NEAT_GRAPHNETWORK_H
#define NEAT_GRAPHNETWORK_H

#include <set>
#include <random>

#include "../neat/NetworkGenome.h"

/**
 * Utility class containing graphing algorithms and used for visualisation.
 */
class GraphNetwork {
private:
    /**
     * Used in calculating evaluation order (see evaluation_order)
     * @param order
     */
    void mark_evaluation_order(int node, std::vector<int> &order) const;
public:
    /**
     * Map containing connections leading to each node.
     * First element of pair is node number, second connection weight.
     * If a weight is infinity, that means it's disabled.
     */
    std::map<int, std::vector<std::pair<int, double>>> connections;
    std::set<int> nodes;
    std::set<int> input;
    std::set<int> output;

    /**
     * Create GraphNetwork from genes in genome.
     * @param genome
     */
    explicit GraphNetwork(const NetworkGenome &genome);

    /**
     * Check if there is a direct connection from a to b.
     * @param a node number
     * @param b node number
     * @return true if there is a direct connection from a to b, otherwise false
     */
    [[nodiscard]] bool connection_exists(int a, int b) const;

    /**
     * Check if there exists a path from a to b.
     * @param a node number
     * @param b node number
     * @return true if there exists a path from a to b, otherwise false
     */
    [[nodiscard]] bool leads_to(int a, int b) const;

    /**
     * Calculate the order in which nodes should be evaluated.
     * Output nodes are always at the beginning and input nodes at the end.
     * @return array of size node.size() containing the order of evaluation (allocated by new)
     */
    [[nodiscard]] int *evaluation_order() const;

    /**
     * Calculate a set of nodes that lead into a given node.
     * It is a set of nodes needed to calculate a value of a given node.
     * @param node given node
     * @return  a set of nodes leading into node
     */
    [[nodiscard]] std::set<int> get_previous_nodes(int node) const;

    /**
     * Get a random connection that is not present in the network.
     * This connection must not create loops. Also it can't lead to input nodes or come from output nodes.
     * @param engine random number engine
     * @return pair containing the connection (first is from, second is to)(std::pair<int, int>() if cannot be found)
     */
    [[nodiscard]] std::pair<int, int> get_new_random_connection(std::default_random_engine& engine) const;

    /**
     * Get a random connection that is not present in the network leading from a given from.
     * @param from from from
     * @param engine random number engine
     * @return pair containing the connection (first is from, second is to) (std::pair<int, int>() if cannot be found)
     */
    [[nodiscard]] std::pair<int, int> get_new_random_connection_from(int from, std::default_random_engine& engine) const;


    /**
     * Get random node from given set.
     * @param set given set
     * @param engine random engine
     * @return a random node
     */
    static int random_from_set(const std::set<int> &set, std::default_random_engine& engine);

    /**
     * Calculate the biggest id of nodes.
     * @return biggest if
     */
    [[nodiscard]] int max_node() const;

    /**
     * Calculate such layers for a network that every connection leads to some following layer.
     * @return vector containing layers
     */
    [[nodiscard]] std::vector<std::vector<int>> get_layers() const;
};


#endif