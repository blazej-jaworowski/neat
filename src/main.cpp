#include <iostream>

#include "neat/Population.h"
#include "graphics/Graphics.h"
#include "utils/FastNetwork.h"

#include <thread>

int main() {
    auto p = Graphics::create_creature();
    Creature preview(p.first, p.second);
    Graphics::simulate_creature(preview);

    auto eval = [&p](std::vector<NetworkGenome> &genomes) {
        std::vector<std::thread> threads;
        threads.reserve(genomes.size());
        for (auto &genome: genomes) {
            threads.emplace_back([&p](NetworkGenome *genome) {
                Creature creature(p.first, p.second, FastNetwork(*genome));
                for (int i = 0; i < 500; i++) {
                    creature.timestep(0.01);
                }
                genome->fitness = std::max(0.0, 1.0 + creature.distance_ran());
//                genome->fitness = creature.highest_jump;
            }, &genome);

        }
        for(auto& thread : threads) {
            thread.join();
        }
    };

    Population population(150, (int)p.first.size() + 1, 1,
                          std::function<void(std::vector<NetworkGenome> &)>(eval));
    for (int i = 0; i < 100; i++) {
        std::cout << i << ": " << population.best_fitness << ", " << population.average_fitness << ", "
                  << population.species.size() << std::endl;
        population.evolution_step();
    }
    Creature creature(p.first, p.second, FastNetwork(*population.best));

    while (true) {
        Graphics::simulate_creature(creature);
    }

    std::cout << "Dobra zmiana" << std::endl;

    return 0;
}
