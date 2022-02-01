#ifndef NEAT_NETWORKGENOME_H
#define NEAT_NETWORKGENOME_H

#include <map>
#include <string>
#include <vector>

#include "Population.h"
#include "Gene.h"

class Population;

class NetworkGenome {
private:
    /**
     * Construct NetworkGenome with given genes.
     * @param input_count	number of inputs
     * @param output_count	number of outputs
     * @param population    population containing the genome
     * @param map           genome
     */
    NetworkGenome(int input_count, int output_count, Population &population, std::map<int, Gene> map);

    /**
     * Calculate the biggest node id in the genome.
     * @return biggest id
     */
    [[nodiscard]] int max_node_id() const;

    /**
     * Calculate the smallest node id that is not in this genome.
     * @return smallest node id that is not in this genome
     */
    [[nodiscard]] int first_available_node_id() const;

    /**
     * Get a random gene from this genome.
     * Genome must not be empty.
     * @return a random gene
     */
    Gene &random_gene();

    /**
     * Calculate the biggest innovation number in this genome.
     * @return biggest innovation number
     */
    [[nodiscard]] int max_innovation_number() const;

public:
    /**
     * Population containing the genome
     */
    Population &population;

    /**
     * Fitness of a genome. (updated by Population)
     */
    double fitness;

    /**
     * A map containing genes, mapping innovation number to a gene.
     */
    std::map<int, Gene> genome;
    const int input_count;
    const int output_count;

    /**
     * Calculate a number of nodes.
     * @return a number of nodes
     */
    [[nodiscard]] int node_count() const;

    /**
     * Create a string representation of the genome.
     * @return string representation of the genome
     */
    [[nodiscard]] std::string print_genome() const;

    /**
     * Crete NetworkGenome with given number of inputs and outputs that are fully connected.
     * @param input_count   number of inputs
     * @param output_count  number of outputs
     * @param population    population containing the genome
     */
    NetworkGenome(int input_count, int output_count, Population &population);

    /**
     * Add a gene with given input, output and weight, assigning it an innovation number.
     * @param in        input node
     * @param out       output node
     * @param weight    weight of the connection
     */
    void add_gene(int in, int out, double weight);

    /**
     * Set a genome's weight to a random value.
     * @param gene
     */
    void mutate_set_connection_weight(Gene &gene);

    /**
     * Perturb a genome's weight by some amount.
     * @param gene
     */
    void mutate_perturb_connection_weight(Gene &gene);

    /**
     * Perform weight mutation on a random genome. (see NEAT paper)
     */
    void mutate_connection_weight();

    /**
     * Perform add node mutation. (see NEAT paper)
     */
    void mutate_add_node();

    /**
     * Perform add connection mutation. (see NEAT paper)
     */
    void mutate_add_connection();

    /**
     * Enable a random gene. (may do nothing if gene already enabled)
     */
    void mutate_enable_connection();

    /**
     * Perform a crossover between two genomes.
     * Both genomes must have the same number of inputs and outputs and the same population.
     * @param parent1 parent with more fitness
     * @param parent2 parent with less fitness
     * @return child genome
     */
    static NetworkGenome crossover(const NetworkGenome &parent1, const NetworkGenome &parent2);

    /**
     * Calculate compatibility distance of two genomes (see NEAT paper)
     * @param genome1
     * @param genome2
     * @param c1 coefficient 1
     * @param c2 coefficient 2
     * @param c3 coefficient 3
     * @return compatibility distance
     */
    static double get_compatibility_distance(const NetworkGenome &genome1, const NetworkGenome &genome2,
                                             double c1 = 1.0, double c2 = 1.0, double c3 = 0.4);

    NetworkGenome &operator=(const NetworkGenome &g);
};

#endif
