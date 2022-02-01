#ifndef NEAT_VECTOR2D_H
#define NEAT_VECTOR2D_H

#include <SFML/Graphics.hpp>

/**
 * Class for vector operations.
 */
class Vector2D {
public:
    double x = 0;
    double y = 0;

    Vector2D &operator+=(const Vector2D& vector);
    Vector2D &operator*=(double d);
    [[nodiscard]] Vector2D operator*(double d) const;
    [[nodiscard]] Vector2D operator/(double d) const;
    [[nodiscard]] Vector2D operator-(const Vector2D& vector) const;
    [[nodiscard]] Vector2D operator+(const Vector2D& vector) const;
    [[nodiscard]] double length() const;

    [[nodiscard]] sf::Vector2f to_screen_space(int width, int height) const;

    Vector2D(double x, double y);
    Vector2D() = default;
};


#endif //NEAT_VECTOR2D_H
