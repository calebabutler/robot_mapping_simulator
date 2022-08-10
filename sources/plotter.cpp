
// Includes
#include "plotter.h"

Plotter::Plotter(sf::VideoMode mode, int grid_width, int grid_height)
    : m_window(mode, "Robot Mapping Simulator")
{
    m_grid_width = grid_width;
    m_grid_height = grid_height;
}

bool Plotter::is_open() const
{
    return m_window.isOpen();
}

void Plotter::handle_events()
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
    }
}

void Plotter::plot(sf::Vector2i robot_position, int robot_orientation, std::vector<sf::Vector2i> obstacles)
{
    m_window.clear(sf::Color(170, 170, 170));

    plot_grid_lines();
    plot_robot(robot_position, robot_orientation);
    plot_obstacles(obstacles);

    m_window.display();
}

void Plotter::plot_grid_lines()
{
    // Get window size
    sf::Vector2u size = m_window.getSize();
    int width = size.x;
    int height = size.y;

    // Create vector of vertices, every two verticies is a single grid line
    std::vector<sf::Vertex> lines;
    lines.reserve((m_grid_width + m_grid_height) * 2);

    // Populate lines vector
    for (int i = 1; i < m_grid_width; i++) {
        float x = static_cast<float>(width) / m_grid_width * i;
        lines.push_back(sf::Vertex(sf::Vector2f(x, 0)));
        lines.push_back(sf::Vertex(sf::Vector2f(x, height)));
    }
    for (int i = 1; i < m_grid_height; i++) {
        float y = static_cast<float>(height) / m_grid_height * i;
        lines.push_back(sf::Vertex(sf::Vector2f(0, y)));
        lines.push_back(sf::Vertex(sf::Vector2f(width, y)));
    }

    // Set color of all grid lines
    for (sf::Vertex& vertex : lines) {
        vertex.color = sf::Color(70, 70, 70);
    }

    // Draw grid lines
    m_window.draw(lines.data(), lines.size(), sf::Lines);
}

void Plotter::plot_robot(sf::Vector2i robot_position, int robot_orientation)
{
    // Get window size
    sf::Vector2u size = m_window.getSize();
    int width = size.x;
    int height = size.y;

    // Find radius
    float cell_width = static_cast<float>(width) / m_grid_width;
    float cell_height = static_cast<float>(height) / m_grid_height;
    float radius = std::min(cell_width / 2, cell_height / 2) - 20;

    // Find x and y values
    float x = (robot_position.x + 0.5) * cell_width;
    float y = height - (robot_position.y + 0.5) * cell_height;

    // Draw triangle
    sf::CircleShape robot(radius, 3);
    robot.setPosition(x, y);
    robot.setOrigin(radius, radius);
    robot.setRotation(robot_orientation * -90);
    robot.setFillColor(sf::Color(70, 70, 170));
    m_window.draw(robot);
}

void Plotter::plot_obstacles(std::vector<sf::Vector2i> obstacles)
{
    // Get window size
    sf::Vector2u size = m_window.getSize();
    int width = size.x;
    int height = size.y;

    // Find radius
    float cell_width = static_cast<float>(width) / m_grid_width;
    float cell_height = static_cast<float>(height) / m_grid_height;
    float radius = std::min(cell_width / 2, cell_height / 2) - 20;

    for (sf::Vector2i& obstacle : obstacles) {
        // Find x and y values
        float x = (obstacle.x + 0.5) * cell_width;
        float y = height - (obstacle.y + 0.5) * cell_height;

        sf::CircleShape obstacle_shape(radius, 4);
        obstacle_shape.setOrigin(radius, radius);
        obstacle_shape.setPosition(x, y);
        obstacle_shape.setFillColor(sf::Color(170, 70, 70));
        m_window.draw(obstacle_shape);
    }
}
