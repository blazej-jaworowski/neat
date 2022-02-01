#ifndef NEAT_STICK_H
#define NEAT_STICK_H


#include "Point.h"

/**
 * Class representing sticks between points.
 */
class Stick {
public:
    Point *ends[2];
    double desired_length;

    Stick(Point &p1, Point &p2, double desired_length);

    void constrain_points();
};


#endif //NEAT_STICK_H
