#include "Vector2D.h"

#include <cmath>

Vector2D &Vector2D::operator+=(const Vector2D& vector) {
    x += vector.x;
    y += vector.y;
    return *this;
}

Vector2D Vector2D::operator*(double d) const {
    return {x * d, y * d};
}

Vector2D Vector2D::operator/(double d) const {
    return {x / d, y / d};
}

Vector2D Vector2D::operator-(const Vector2D &vector) const {
    return {x - vector.x, y - vector.y};
}

double Vector2D::length() const {
    return std::sqrt(x * x + y * y);
}

sf::Vector2f Vector2D::to_screen_space(int width, int height) const {
    return {(float)(1.0 * width / 2 + x * 100), (float)(2.0 * height / 3 - y * 100)};
}

Vector2D &Vector2D::operator*=(double d) {
    x *= d;
    y *= d;
    return *this;
}

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

Vector2D Vector2D::operator+(const Vector2D &vector) const {
    return {x + vector.x, y + vector.y};
}
