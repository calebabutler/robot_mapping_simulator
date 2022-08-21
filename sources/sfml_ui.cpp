
// Includes
#include "sfml_ui.h"
#include <iostream>

// Global constants
const sf::Vector2u WINDOW_SIZE = {1000, 1000};
const char * const WINDOW_TITLE = "Robot Mapping Simulator";

SFMLUI::SFMLUI(const Parameters& parameters) : m_app(parameters)
{
    m_window.create(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), WINDOW_TITLE);
}

int SFMLUI::run_loop()
{
    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
        }
        m_app.step_through();
        sf::sleep(sf::milliseconds(100));
        // Draw
        m_window.clear(sf::Color(170, 170, 170));
        draw_grid_lines();
        draw_robot();
        draw_obstacles();
        m_window.display();
    }
    return 0;
}

void SFMLUI::draw_grid_lines()
{
    // Create vector of vertices, every two verticies is a single grid line
    std::vector<sf::Vertex> lines;
    lines.reserve((m_app.get_grid_width() + m_app.get_grid_height()) * 2);

    // Populate lines vector
    for (int i = 1; i < m_app.get_grid_width(); i++) {
        float x = static_cast<float>(WINDOW_SIZE.x) / m_app.get_grid_width() * i;
        lines.push_back(sf::Vertex(sf::Vector2f(x, 0)));
        lines.push_back(sf::Vertex(sf::Vector2f(x, WINDOW_SIZE.y)));
    }
    for (int i = 1; i < m_app.get_grid_height(); i++) {
        float y = static_cast<float>(WINDOW_SIZE.y) / m_app.get_grid_height() * i;
        lines.push_back(sf::Vertex(sf::Vector2f(0, y)));
        lines.push_back(sf::Vertex(sf::Vector2f(WINDOW_SIZE.x, y)));
    }

    // Set color of all grid lines
    for (sf::Vertex& vertex : lines) {
        vertex.color = sf::Color(70, 70, 70);
    }

    // Draw grid lines
    m_window.draw(lines.data(), lines.size(), sf::Lines);
}

void SFMLUI::draw_robot()
{
    // Find radius
    float cell_width = static_cast<float>(WINDOW_SIZE.x) / m_app.get_grid_width();
    float cell_height = static_cast<float>(WINDOW_SIZE.y) / m_app.get_grid_height();
    float radius = std::min(cell_width / 2, cell_height / 2) - 20;

    // Find x and y values
    float x = (m_app.get_robot_position().x + 0.5) * cell_width;
    float y = WINDOW_SIZE.y - (m_app.get_robot_position().y + 0.5) * cell_height;

    // Draw triangle
    sf::CircleShape robot(radius, 3);
    robot.setPosition(x, y);
    robot.setOrigin(radius, radius);
    robot.setRotation(m_app.get_robot_orientation() * -90);
    robot.setFillColor(sf::Color(70, 70, 170));
    m_window.draw(robot);
}

void SFMLUI::draw_obstacles()
{
    // Find radius
    float cell_width = static_cast<float>(WINDOW_SIZE.x) / m_app.get_grid_width();
    float cell_height = static_cast<float>(WINDOW_SIZE.y) / m_app.get_grid_height();
    float radius = std::min(cell_width / 2, cell_height / 2) - 20;

    for (Vector2 obstacle : m_app.get_found_obstacles()) {
        // Find x and y values
        float x = (obstacle.x + 0.5) * cell_width;
        float y = WINDOW_SIZE.y - (obstacle.y + 0.5) * cell_height;

        sf::CircleShape obstacle_shape(radius, 4);
        obstacle_shape.setOrigin(radius, radius);
        obstacle_shape.setPosition(x, y);
        obstacle_shape.setFillColor(sf::Color(170, 70, 70));
        m_window.draw(obstacle_shape);
    }
}
