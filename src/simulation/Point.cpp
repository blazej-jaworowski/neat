#include <cmath>
#include "Point.h"

void Point::timestep(double delta) {
    Vector2D velocity = (position - old_position) / delta;
    force += Vector2D(0, -0.9);
    if (velocity.x > 0) {
        force.x -= pressure / 10;
    } else if (velocity.x < 0) {
        force.x += pressure / 10;
    }
    old_position = position;
    position += (velocity + force) * delta;
    force *= 0;
}

Point::Point(const Vector2D &position) : position(position), old_position(position) {}
