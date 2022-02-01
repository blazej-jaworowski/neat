#ifndef NEAT_CREATURE_H
#define NEAT_CREATURE_H

#include <vector>
#include "Point.h"
#include "Stick.h"
#include "../utils/FastNetwork.h"

/**
 * Class used for simulating creature physics.
 */
class Creature {
public:
    std::vector<Point> points;
    std::vector<Stick> sticks;
    FastNetwork network;

    double decision_period = 0.1;
    double time_until_decision = 0;

    double energy_spent = 0;
    double highest_jump = 0;

    [[nodiscard]] double distance_ran() const;
    Creature(const std::vector<Vector2D> &points, const std::vector<std::pair<int, int>> &connections, const FastNetwork &network);
    Creature(const std::vector<Vector2D> &points, const std::vector<std::pair<int, int>> &connections);

    void timestep(double delta);
    void constrain_points(double delta);
    void normalise_position();
    void take_action();

    Creature(const Creature&) = delete;
    Creature(const Creature&&) = delete;
};


#endif //NEAT_CREATURE_H
