#include "Creature.h"
#include "../utils/FastNetwork.h"

#include <cmath>
#include <iostream>

void Creature::take_action() {
    return;
    auto *inputs = new double[network.input_count];

    for(int i = 0; i < points.size(); i++) {
        inputs[i] = points.at(i).pressure;
    }
    inputs[network.input_count - 1] = 1;
    double *outputs = network.calculate(inputs);
    delete[] inputs;
}

void Creature::timestep(double delta) {
    if (network.input_count > 0 && (time_until_decision -= delta) <= 0) {
        time_until_decision = decision_period;
        take_action();
    }

    for (auto &point: points) {
        point.timestep(delta);
    }

    constrain_points(delta);

    double jump_height = (*std::min_element(points.begin(), points.end(), [](const auto &point1, const auto &point2) {
        return point1.position.y - point2.position.y;
    })).position.y;

    if (jump_height > highest_jump) highest_jump = jump_height;
}

Creature::Creature(const std::vector<Vector2D> &points, const std::vector<std::pair<int, int>> &connections,
                   const FastNetwork &network) : Creature(points, connections) {
    this->network = network;
}

Creature::Creature(const std::vector<Vector2D> &points, const std::vector<std::pair<int, int>> &connections)
        : network() {
    for (const auto &p: points) {
        this->points.emplace_back(p);
    }
    normalise_position();
    for (const auto&[from, to]: connections) {
        Point &p1 = this->points.at(from);
        Point &p2 = this->points.at(to);
        sticks.emplace_back(p1, p2, (p1.position - p2.position).length());
    }
}

double Creature::distance_ran() const {
    return std::max_element(points.begin(), points.end(), [](const Point p1, const Point p2) {
        return p1.position.x - p2.position.x;
    })->position.x;
}

void Creature::normalise_position() {
    double minx = points.front().position.x;
    double maxx = points.front().position.x;
    double miny = points.front().position.y;
    for (const auto &p: points) {
        if (p.position.x < minx) minx = p.position.x;
        if (p.position.y < miny) miny = p.position.y;
        if (p.position.x > maxx) maxx = p.position.x;
    }
    double centerx = (minx + maxx) / 2;
    for (auto &p: points) {
        p.position.x -= centerx;
        p.old_position.x -= centerx;
        p.position.y -= miny;
        p.old_position.y -= miny;
    }
}

void Creature::constrain_points(double delta) {
    for (int i = 0; i < 5; i++) {
        for (auto &stick: sticks) {
            stick.constrain_points();
        }
    }
    for (auto &point: points) {
        if (point.position.y < 0) {
            point.pressure = -point.position.y / delta;
            point.position.y = 0;
        } else {
            point.pressure = 0;
        }
    }
}
