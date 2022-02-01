#ifndef NEAT_POINT_H
#define NEAT_POINT_H

#include "Vector2D.h"

/**
 * Class containing joint information.
 */
class Point {
public:
    Vector2D position;
    Vector2D old_position;
    Vector2D force;

    double pressure = 0;

    explicit Point(const Vector2D& position);

    void timestep(double delta);
};


#endif //NEAT_POINT_H
