#ifndef NEAT_SPECIES_H
#define NEAT_SPECIES_H

#include <vector>

#include "Population.h"
#include "NetworkGenome.h"

class Population;

class NetworkGenome;

/**
 * Class containing species information
 */
class Species {
public:
    Population *population;
    NetworkGenome *representative;
    std::vector<NetworkGenome *> genomes;
    double fitness = 0;
    double max_fitness = 0;
    int generations_left = 15;

    /**
     * Create new species with a given representative
     * @param genome species representative
     */
    Species(NetworkGenome &genome);

    Species(const Species &species);

    /**
     * Checks if a genome is compatible with this species based on threshold contained in population
     * @param genome
     * @return true if genome is compatible, false otherwise
     */
    [[nodiscard]] bool genome_compatible(NetworkGenome &genome) const;

    /**
     * Inserts a genome into the species if it is compatible
     * @param genome
     * @return true if genome was inserted, false otherwise
     */
    bool insert_genome(NetworkGenome &genome);

    /**
     * Reduce fitness based on number of genomes in a species. (see NEAT paper)
     * Also calculate total fitness.
     */
    void normalise_fitness();

    /**
     * Choose a random genome in the species. (all genomes have the same probability)
     * @return a random genome
     */
    const NetworkGenome *random_genome();

    /**
     * Adds n new genomes made from crossover of random members of this species.
     * @param n number of new genomes
     * @param p vector of genomes to add to
     */
    void get_offspring(int n, std::vector<NetworkGenome> &p);

    /**
     * Sort genomes by fitness.
     * Remove the worst performing organisms.
     */
    void reduce_population();

    Species &operator=(const Species& species);

    ~Species();
};


#endif //NEAT_SPECIES_H
