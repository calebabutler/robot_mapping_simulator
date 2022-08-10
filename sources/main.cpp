
#include "plotter.h"
#include <vector>
#include <SFML/Graphics.hpp>

int main()
{
    Plotter plotter(sf::VideoMode(1000, 1000), 2, 4);
    sf::Vector2i robot_position(0, 3);
    std::vector<sf::Vector2i> obstacles = {
        sf::Vector2i(0, 0),
        sf::Vector2i(1, 1),
        sf::Vector2i(1, 2),
    };

    while (plotter.is_open()) {
        plotter.handle_events();
        plotter.plot(robot_position, 3, obstacles);
    }
    return 0;
}
