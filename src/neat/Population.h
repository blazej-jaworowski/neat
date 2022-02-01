#ifndef NEAT_POPULATION_H
#define NEAT_POPULATION_H

#include <tuple>
#include <vector>
#include <random>
#include <functional>

#include "NetworkGenome.h"
#include "Gene.h"
#include "Species.h"

class Species;

class Population {
private:
    const int size; /// Population size
public:
    double enable_gene_chance = 0.25;
    double weight_mutation_chance = 0.8;
    double set_weight_chance = 0.1;
    double add_node_mutation_chance = 0.03;
    double add_connection_mutation_chance = 0.05;

    double non_crossover_breeding_rate = 0.25;
    double selection_rate = 0.25;

    double compatibility_threshold = 3.0;
    double c1 = 1.0;
    double c2 = 1.0;
    double c3 = 0.4;

    const NetworkGenome *best = nullptr;
    double best_fitness = 0;
    double average_fitness = 0;

    /**
     * Function evaluating all genomes (setting their fitness).
     */
    std::function<void(std::vector<NetworkGenome> &)> evaluation;

    std::vector<NetworkGenome> genomes;

    std::vector<Species> species;

    /**
     * Vector of unique mutations this generation. Only in, out and innovation number matter.
     */
    std::vector<Gene> innovations;

    int innovation_number = 0;

    std::default_random_engine random_generator;

    /**
     * Create a random population of size genomes. Genomes have default topologies.
     * @param size size of the population
     * @param inputs input count of genomes
     * @param outputs output count of genomes
     */
    Population(int size, int inputs, int outputs, std::function<void(std::vector<NetworkGenome> &)> evaluation);

    /**
     * Get innovation number for genome containing connection from in to out.
     * If there has been the same mutation this generation, the same innovation number should be assigned,
     * otherwise a new innovation number is assgned and the global innovation number is incremented.
     * (see NEAT paper)
     * @param in
     * @param out
     * @return innovation number
     */
    int get_innovation_number(int in, int out);

    /**
     * Calculate a random connection weight.
     * @return connection weight
     */
    double random_weight();

    /**
     * Calculate a random perturbation of a connection weight
     * @return perturbation of a connection weight
     */
    double random_perturbation();

    /**
     * Mutate all genomes in the population
     */
    void mutate();

    /**
     * Clear species, assign each genome to a species, then manage species.
     */
    void speciate();

    /**
     * Insert genome into species.
     * @param genome
     */
    void insert_into_species(NetworkGenome &genome);

    /**
     * Evaluate genome fitness.
     */
    void evaluate();

    /**
     * Reduce fitness based on number of genomes in a species. (see NEAT paper)
     */
    void normalise_fitness();

    /**
     * Replace current genome population with a new generation.
     */
    void next_generation();

    /**
     * Choose a random genome based on all genome fitnesses.
     * (more fit genomes are more likely to be chosen)
     * @return a random genome
     */
    const NetworkGenome &random_genome();

    /**
     * Choose a random species based on all species fitnesses.
     * (species with higher average fitness is more likely to be chosen)
     * @return a random species
     */
    Species &random_species();

    /**
     * Evaluate and normalise genome fitnesses, then create new generation.
     */
    void evolution_step();
};


#endif
