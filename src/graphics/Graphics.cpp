#include <sstream>
#include <iostream>

#include "Graphics.h"

sf::Color Graphics::get_color(double weight) {
    if (weight == INFINITY) {
        // Connection disabled
        return {128, 128, 128};
    }

    uint8_t r = 255;
    uint8_t g = 255;

    // Scale weight
    weight = (int) (255 * weight);

    // Limit weight
    if (weight > 255) weight = 255;
    if (weight < -255) weight = -255;

    // If weight negative remove from green, if positive remove from red
    if (weight < 0) {
        g += (uint8_t) weight;
    } else {
        r -= (uint8_t) weight;
    }

    return {r, g, 0};
}


void Graphics::show_network(const GraphNetwork &network) {
    //Create window
    sf::RenderWindow window(sf::VideoMode(640, 480), "NEAT");
    window.clear();

    const int width = (int) window.getSize().x;
    const int height = (int) window.getSize().y;

    // Alternative
    // auto positions = get_random_positions(network, width, height);

    auto positions = get_layered_positions(network, width, height);

    // Iterate through all nodes
    for (int node: network.nodes) {
        // Draw connections
        for (const auto &connection: network.connections.at(node)) {
            const sf::Color color = get_color(connection.second);

            const sf::Vertex line[] = {
                    sf::Vertex(positions.at(node), color),
                    sf::Vertex(positions.at(connection.first), color)
            };

            window.draw(line, 2, sf::Lines);
        }

        // Draw node

        // Load font
        sf::Font font;
        font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf");

        // Draw text
        sf::Text text;
        text.setCharacterSize(24);
        text.setFont(font);
        std::stringstream ss;
        ss << node;
        text.setString(ss.str());
        text.setPosition(positions.at(node));
        window.draw(text);

        // Draw circle
        sf::CircleShape circle(2);
        circle.setPosition(positions.at(node).x - circle.getRadius() / 2,
                           positions.at(node).y - circle.getRadius() / 2);
        window.draw(circle);
    }

    window.display();

    // Event loop
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // Exit
                window.close();
            }
        }
        sf::sleep(sf::milliseconds(1));
    }
}

std::map<int, sf::Vector2f> Graphics::get_layered_positions(const GraphNetwork &network, int width, int height) {
    std::map<int, sf::Vector2f> positions;

    const auto layers = network.get_layers();

    std::default_random_engine engine(12345);
    std::uniform_real_distribution<float> distribution(-50, 50);

    for (int i = 0; i < layers.size(); i++) {
        const std::vector<int> &current_layer = layers[i];
        float x = (float) width * (float) (i + 1) / (float) (layers.size() + 1);

        for (int j = 0; j < current_layer.size(); j++) {
            float y = (float) height * (float) (j + 1) / (float) (current_layer.size() + 1) + distribution(engine);

            positions[layers[i][j]] = sf::Vector2f(x, y);
        }
    }

    return positions;
}

void Graphics::simulate_creature(Creature &creature) {
    creature.normalise_position();

    sf::RenderWindow window(sf::VideoMode(640, 480), "Simulation");

    double timestep = 0.01;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        int width = (int) window.getSize().x;
        int height = (int) window.getSize().y;

        for (const auto &point: creature.points) {
            sf::CircleShape p(4);
            sf::Vector2f pos = point.position.to_screen_space(width, height);
            pos.x -= 4;
            pos.y -= 4;
            p.setPosition(pos);
            window.draw(p);
        }

        for (const auto &stick: creature.sticks) {
            sf::Vertex vertices[] = {
                    {stick.ends[0]->position.to_screen_space(width, height), sf::Color::White},
                    {stick.ends[1]->position.to_screen_space(width, height), sf::Color::White}
            };

            window.draw(vertices, 2, sf::Lines);
        }
        sf::Vertex ground[] = {
                sf::Vertex({0, (float) (2.0 * height / 3)}),
                sf::Vertex({(float) width, (float) (2.0 * height / 3)})
        };
        window.draw(ground, 2, sf::Lines);

        window.display();

        creature.timestep(timestep);

        sf::sleep(sf::milliseconds((int) (1000 * timestep)));
    }
}

std::pair<std::vector<Vector2D>, std::vector<std::pair<int, int>>> Graphics::create_creature() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Draw creature");

    std::vector<Vector2D> points;
    std::vector<std::pair<int, int>> connections;

    Vector2D mouse_press;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    mouse_press = Vector2D(event.mouseButton.x, event.mouseButton.y);
                    break;
                case sf::Event::MouseButtonReleased:
                    Vector2D pos(event.mouseButton.x, event.mouseButton.y);
                    if ((mouse_press - pos).length() <= 8) {
                        points.push_back(pos);
                        std::cout << pos.x << ", " << pos.y << std::endl;
                    } else {
                        int from = -1;
                        int to = -1;
                        for (int i = 0; i < points.size(); i++) {
                            if ((points[i] - pos).length() <= 8) {
                                to = i;
                            }
                            if ((points[i] - mouse_press).length() <= 8) {
                                from = i;
                            }
                        }
                        if (from == -1 || to == -1) break;
                        connections.emplace_back(from, to);
                        std::cout << from << ", " << to << std::endl;
                    }
                    break;
            }
        }

        window.clear();

        for (const auto &p: points) {
            sf::CircleShape c(8);
            sf::Vector2f pos((float) p.x, (float) p.y);
            pos.x -= 8;
            pos.y -= 8;
            c.setPosition(pos);
            window.draw(c);
        }

        for (const auto &[from, to]: connections) {
            sf::Color color = sf::Color::White;
            sf::Vertex vertices[] = {
                    {sf::Vector2f(points[from].x, points[from].y), color},
                    {sf::Vector2f(points[to].x, points[to].y),     color}
            };

            window.draw(vertices, 2, sf::Lines);
        }

        window.display();
        sf::milliseconds(1);
    }

    for (auto &p: points) {
        p *= 0.01;
        p.y *= -1;
    }

    return {points, connections};
}

