
// Begin header guard
#ifndef PLOTTER_H
#define PLOTTER_H

// Includes
#include <vector>
#include <SFML/Graphics.hpp>

class Plotter {
private:
    sf::RenderWindow m_window;
    int m_grid_width;
    int m_grid_height;
    // Helper functions
    void plot_grid_lines();
    void plot_robot(sf::Vector2i robot_position, int robot_orientation);
    void plot_obstacles(std::vector<sf::Vector2i> obstacles);
public:
    Plotter(sf::VideoMode mode, int grid_width, int grid_height);
    bool is_open() const;
    void handle_events();
    void plot(sf::Vector2i robot_position, int robot_orientation, std::vector<sf::Vector2i> obstacles);
};

// End header guard
#endif
