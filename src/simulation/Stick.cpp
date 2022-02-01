#include "Stick.h"

Stick::Stick(Point &p1, Point &p2, double desired_length) : ends{&p1, &p2}, desired_length(desired_length) {}

void Stick::constrain_points() {
    Vector2D dist = ends[1]->position - ends[0]->position;
    double d = dist.length();
    double correction = (d - desired_length) / d / 2;

    ends[0]->position += dist * correction;
    ends[1]->position += dist * -correction;
}