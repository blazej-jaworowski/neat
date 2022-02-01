#ifndef NEAT_GRAPHICS_H
#define NEAT_GRAPHICS_H

#include <SFML/Graphics.hpp>

#include "../utils/GraphNetwork.h"
#include "../simulation/Creature.h"

/**
 * Class for all graphics related stuff.
 */
class Graphics {
public:
    /**
     * Create a window visualising a neural network.
     * @param network FastNetwork to visualize.
     */
    [[maybe_unused]] static void show_network(const GraphNetwork& network);

    /**
     * Calculate a color of a connection with given weight
     * @param weight connection weight
     * @return color of connection
     */
    static sf::Color get_color(double weight);

    /**
     * Calculate positions for nodes of a network,
     * Nodes will be positioned in layers where every connection leads to some following layer.
     * @param network
     * @param width width of the window
     * @param height height of the window
     * @return map from node id to position
     */
    static std::map<int, sf::Vector2f> get_layered_positions(const GraphNetwork& network, int width, int height);

    static void simulate_creature(Creature &creature);

    static std::pair<std::vector<Vector2D>, std::vector<std::pair<int, int>>> create_creature();
};


#endif
