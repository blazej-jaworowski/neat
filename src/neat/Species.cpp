#include <iostream>
#include "Species.h"

bool Species::genome_compatible(NetworkGenome &genome) const {
    return NetworkGenome::get_compatibility_distance(genome, *representative,
                                                     genome.population.c1, genome.population.c2,
                                                     genome.population.c3) <= population->compatibility_threshold;
}

bool Species::insert_genome(NetworkGenome &genome) {
    if (!genome_compatible(genome)) return false;

    genomes.push_back(&genome);
    return true;
}

Species::Species(NetworkGenome &genome) : population(&genome.population) {
    representative = new NetworkGenome(genome);
    genomes.push_back(&genome);
}

Species::~Species() {
    delete representative;
}

Species::Species(const Species &species) : genomes(species.genomes), population(species.population),
                                            representative(new NetworkGenome(*species.representative)) {
}

void Species::normalise_fitness() {
    auto size = genomes.size();
    fitness = 0;
    for(auto genome : genomes) {
        fitness += genome->fitness;
        genome->fitness /= (double)size;
    }
    fitness /= (double)size;
}

const NetworkGenome *Species::random_genome() {
    std::uniform_int_distribution<int> distribution(0, (int)genomes.size() - 1);
    return genomes.at(distribution(population->random_generator));
}

Species &Species::operator=(const Species &species) {
    if (this == &species) return *this;
    population = species.population;
    representative = new NetworkGenome(*species.representative);
    genomes = species.genomes;
    fitness = species.fitness;
    return *this;
}

void Species::get_offspring(int n, std::vector<NetworkGenome> &p) {
    int non_crossover = (int)((double)n * population->non_crossover_breeding_rate);

    for(int i = 0; i < non_crossover; i++) {
        p.push_back(*random_genome());
    }

    for(int i = 0; i < n - non_crossover; i++) {
        p.push_back(NetworkGenome::crossover(*random_genome(), *random_genome()));
    }
}

void Species::reduce_population() {
    std::sort(genomes.begin(), genomes.end(), [](const NetworkGenome* genome1, const NetworkGenome* genome2) {
        return (genome1->fitness - genome2->fitness) > 0;
    });
    int survivor_count = std::ceil((double)genomes.size() * population->selection_rate);
    genomes.erase(genomes.begin() + survivor_count, genomes.end());
}
