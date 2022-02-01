#include <iostream>
#include <set>
#include <sstream>
#include <utility>

#include "NetworkGenome.h"
#include "../utils/GraphNetwork.h"

NetworkGenome::NetworkGenome(int input_count, int output_count, Population &population, std::map<int, Gene> map)
        : input_count(input_count),
          output_count(output_count), population(population), genome(std::move(map)) {}

NetworkGenome::NetworkGenome(int input_count, int output_count, Population &population) : input_count(input_count),
                                                                                          output_count(output_count),
                                                                                          population(population) {
    for (int i = 0; i < input_count; i++) {
        for (int j = 0; j < output_count; j++) {
            add_gene(i, input_count + j, population.random_weight());
        }
    }
}

void NetworkGenome::add_gene(int in, int out, double weight) {
    // Calculate innovation number and modify population
    int innovation_number = population.get_innovation_number(in, out);

    // Add gene
    genome[innovation_number] = Gene(in, out, innovation_number, true, weight);
}

void NetworkGenome::mutate_set_connection_weight(Gene &gene) {
    random_gene().weight = population.random_weight();
}

void NetworkGenome::mutate_connection_weight() {
    std::uniform_real_distribution<double> distribution(0, 1);
    if (distribution(population.random_generator) < population.set_weight_chance) {
        mutate_perturb_connection_weight(random_gene());
    } else {
        mutate_set_connection_weight(random_gene());
    }
}

void NetworkGenome::mutate_add_node() {
    // Select random gene
    Gene &g = random_gene();

    // Split it into two connections with new node in between
    int node = first_available_node_id();
    add_gene(g.in, node, 1);
    add_gene(node, g.out, g.weight);

    // Disable gene
    g.enabled = false;
}

int NetworkGenome::first_available_node_id() const {
    const int max_id = max_node_id();

    // Array indicating which ids exist
    bool *z = new bool[max_id + 1];
    std::fill_n(z, max_id + 1, false);

    // Mark existing node ids
    for (const auto &p: genome) {
        z[p.second.in] = true;
        z[p.second.out] = true;
    }

    // Find smallest available id
    for (int i = 0; i < max_id; i++) {
        if (!z[i]) {
            delete[] z;
            return i;
        }
    }

    delete[] z;
    return max_id + 1;
}

int NetworkGenome::max_node_id() const {
    int max = -1;
    for (const auto &p: genome) {
        if (p.second.in > max) max = p.second.in;
        if (p.second.out > max) max = p.second.out;
    }
    return max;
}

std::string NetworkGenome::print_genome() const {
    std::stringstream s;
    for (const auto &p: genome) {
        s << p.second.in << "--[" << p.second.weight << "]->" << p.second.out << (p.second.enabled ? "" : " (disabled)")
          << std::endl;
    }
    s << std::endl;
    return s.str();
}

void NetworkGenome::mutate_add_connection() {
    // Get random connection that doesn't exist
    GraphNetwork graph(*this);
    auto connection = graph.get_new_random_connection(population.random_generator);

    // Network full
    if (connection == std::pair<int, int>()) {
        return;
    }

    if (connection.first == connection.second) {
        std::cout << "X";
    }

    // Add gene with that connection
    add_gene(connection.first, connection.second, population.random_weight());
}

void NetworkGenome::mutate_enable_connection() {
    random_gene().enabled = true;
}

NetworkGenome NetworkGenome::crossover(const NetworkGenome &parent1, const NetworkGenome &parent2) {
    int range = parent1.max_innovation_number();

    // Child's genome
    std::map<int, Gene> genome;

    std::uniform_real_distribution<double> distribution(0, 1);

    // Go through every possible innovation number of fitter parent
    for (int i = 0; i <= range; i++) {
        if (parent1.genome.contains(i)) {
            if (parent2.genome.contains(i)) {
                // Matching genes
                genome[i] =
                        distribution(parent1.population.random_generator) < 0.5 ? parent1.genome.at(i)
                                                                                : parent2.genome.at(i);
            } else {
                // Excess/disjoint genes
                genome[i] = parent1.genome.at(i);
            }

            // Chance to enable gene
            if (distribution(parent1.population.random_generator) < parent1.population.enable_gene_chance) {
                genome[i].enabled = true;
            }
        }
    }

    // Create child
    return {parent1.input_count, parent1.output_count, parent1.population, genome};
}

Gene &NetworkGenome::random_gene() {
    // Get random position of a gene
    std::uniform_int_distribution<int> distribution(0, (int) genome.size() - 1);
    int index = distribution(population.random_generator);

    // Return gene in said position in sequence
    for (auto &p: genome) {
        if (index == 0) {
            return p.second;
        }
        index--;
    }

    // Should never be executed
    return genome[0];
}

int NetworkGenome::max_innovation_number() const {
    int max = -1;
    for (const auto &p: genome) {
        if (p.first > max) max = p.first;
    }
    return max;
}

int NetworkGenome::node_count() const {
    // Set of all nodes
    std::set<int> node_set;

    // Add nodes to the set
    for (const auto &gene: genome) {
        node_set.insert(gene.second.in);
        node_set.insert(gene.second.out);
    }

    return (int) node_set.size();
}

void NetworkGenome::mutate_perturb_connection_weight(Gene &gene) {
    gene.weight += population.random_perturbation();
}

double NetworkGenome::get_compatibility_distance(const NetworkGenome &genome1, const NetworkGenome &genome2, double c1,
                                                 double c2, double c3) {
    int disjoint_sum = 0;
    double weight_difference = 0;
    int matching_sum = 0;

    int max1 = genome1.max_innovation_number();
    int max2 = genome2.max_innovation_number();

    for (int i = 0; i < std::min(max1, max2); i++) {
        if (!genome1.genome.contains(i) && !genome2.genome.contains(i)) continue;
        if (genome1.genome.contains(i) && genome2.genome.contains(i)) {
            matching_sum++;
            weight_difference += std::abs(genome1.genome.at(i).weight - genome2.genome.at(i).weight);
            continue;
        }
        disjoint_sum++;
    }

    int excess_sum = std::abs(max1 - max2);

//    int N = std::min(max1, max2) < 20 ? 1 : (int)std::max(genome1.genome.size(), genome2.genome.size());
    int N = (int)std::max(genome1.genome.size(), genome2.genome.size());

    return c1 * excess_sum / N + c2 * disjoint_sum / N + (matching_sum > 0 ? c3 * weight_difference / matching_sum : 0);
}

NetworkGenome &NetworkGenome::operator=(const NetworkGenome &g) {
    genome = g.genome;
    return *this;
}