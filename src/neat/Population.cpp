#include <iostream>
#include <utility>
#include "Population.h"

Population::Population(int size, int inputs, int outputs, std::function<void(std::vector<NetworkGenome>&)> evaluation) :
        size(size), evaluation(std::move(evaluation)) {
    random_generator = std::default_random_engine(time(NULL));
    for (int i = 0; i < size; i++) {
        genomes.emplace_back(inputs, outputs, *this);
    }

    mutate();
    speciate();
    evaluate();
}

int Population::get_innovation_number(int in, int out) {
    // Search for equal connection
    for (const auto &innovation: innovations) {
        if (innovation.in == in && innovation.out == out) {
            // If connection found return its innovation number
            return innovation.innovation;
        }
    }

    // Insert new connection into vector of mutations this generation
    innovations.emplace_back(in, out, innovation_number, false, 0);

    // Return then increment global innovation number
    return innovation_number++;
}

double Population::random_weight() {
    std::uniform_real_distribution<double> distribution(-1, 1);
    return distribution(random_generator);
}

double Population::random_perturbation() {
    std::uniform_real_distribution distribution(-0.1, 0.1);
    return distribution(random_generator);
}

void Population::mutate() {
    std::uniform_real_distribution<double> distribution(0, 1);
    for (auto &genome: genomes) {
        if (distribution(random_generator) < weight_mutation_chance) genome.mutate_connection_weight();
        if (distribution(random_generator) < add_connection_mutation_chance) genome.mutate_add_connection();
        if (distribution(random_generator) < add_node_mutation_chance) genome.mutate_add_node();
    }
}

void Population::speciate() {
    for (auto &s: species) {
        s.genomes.clear();
    }

    for (auto &genome: genomes) {
        insert_into_species(genome);
    }

    auto r = std::remove_if(species.begin(), species.end(), [](const Species &s) {
        return s.genomes.empty();
    });
    species.erase(r, species.end());
}

void Population::insert_into_species(NetworkGenome &genome) {
    bool inserted = false;
    for (auto &s: species) {
        if (s.insert_genome(genome)) {
            inserted = true;
            break;
        }
    }

    if (!inserted) {
        species.emplace_back(genome);
    }
}

void Population::normalise_fitness() {
    for (auto &s: species) {
        s.normalise_fitness();
    }
}

void Population::evaluate() {
    evaluation(genomes);
    best_fitness = -1;
    average_fitness = 0;
    for (const auto &genome: genomes) {
        if (genome.fitness > best_fitness) {
            best = &genome;
            best_fitness = best->fitness;
        }

        average_fitness += genome.fitness;
    }
    average_fitness /= (double) genomes.size();
    normalise_fitness();
}

void Population::next_generation() {
    std::vector<NetworkGenome> champions;
    champions.push_back(*best);
    for (const auto &s: species) {
        if (s.genomes.size() >= 5) {
            champions.push_back(*s.genomes.front());
        }
    }
    double total_fitness = std::accumulate(species.begin(), species.end(), 0.0, [](double init, const Species &s) {
        return init + s.fitness;
    });
    int remaining = size - (int) champions.size();
    std::vector<NetworkGenome> new_population;
    for (auto &s: species) {

        s.reduce_population();
        int to_add = (int) ((double) (size - champions.size()) * s.fitness / total_fitness);
        s.get_offspring(to_add, new_population);
        remaining -= to_add;
    }

    for (int i = 0; i < remaining; i++) {
        new_population.push_back(NetworkGenome::crossover(random_genome(), random_genome()));
    }

    genomes = new_population;

    mutate();

    genomes.insert(genomes.begin(), champions.begin(), champions.end());
}

void Population::evolution_step() {
    for(auto& s : species) {
        for(const auto& genome : s.genomes) {
            s.generations_left--;
            if(genome->fitness > s.max_fitness) {
                s.generations_left = 15;
            }
        }
    }
    if(species.size() > 1) {
        auto r = std::remove_if(species.begin(), species.end(), [](const Species& s) {
            return s.generations_left <= 0;
        });
        species.erase(r, species.end());
    }

    next_generation();
    speciate();
    evaluate();
}

const NetworkGenome &Population::random_genome() {
    std::uniform_int_distribution<int> distribution(0, size - 1);
    return genomes.at(distribution(random_generator));
}