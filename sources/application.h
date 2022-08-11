
// Begin header guard
#ifndef APPLICATION_H
#define APPLICATION_H

// Includes
#include <string>
#include "data_types.h"
#include "robot_server.h"
#include "plotter.h"

enum class ApplicationMode {
    AUTO,
    STEP_THROUGH,
    HELP,
    DEFAULTS,
    ALGORITHMS,
};

struct Algorithm {
    std::string name;
    void (*sense)(RobotServer&);
    void (*plan)(RobotServer&);
    void (*act)(RobotServer&);
    void (*plot)(RobotServer&, Plotter&);
};

class Application {
private:
    // Standard application data
    int m_window_width;
    int m_window_height;
    int m_grid_width;
    int m_grid_height;
    int m_obstacle_amount;
    std::string m_algorithm_name;
    ApplicationMode m_mode;
    // Robot position and orientation
    Vector2 m_robot_position;
    int m_robot_orientation;
    // Obstacle positions
    std::vector<Vector2> m_obstacles;
    std::vector<Vector2> m_found_obstacles;
    // Algorithms
    std::vector<Algorithm> m_algorithms;
    // Additional data members
    int m_number_of_iterations;
    bool m_keep_running;
    // Private member functions for each mode
    void print_help();
    void print_defaults();
    void print_algorithms();
    void run_simulation();
    // Private member function for running the algorithm
    void generate_random_obstacles();
    void run_algorithm_once(RobotServer&, Plotter&);
    // Private member functions for drawing
    void draw_grid_lines(sf::RenderWindow&);
    void draw_robot(sf::RenderWindow&);
    void draw_obstacles(sf::RenderWindow& window);
public:
    Application();
    void add_algorithm(std::string name, void (*sense)(RobotServer&),
                       void (*plan)(RobotServer&), void (*act)(RobotServer&),
                       void (*plot)(RobotServer&, Plotter&));
    void set_window_width(int);
    void set_window_height(int);
    void set_grid_width(int);
    void set_grid_height(int);
    void set_obstacle_amount(int);
    void set_algorithm(std::string);
    void set_mode(ApplicationMode);
    int run();

    // Member function for Plotter
    void set_found_obstacles(const std::vector<Vector2>&);

    // Member functions for Robot Sim Server
    Vector2 get_robot_position();
    int get_robot_orientation();
    int get_number_of_iterations();
    const std::vector<Vector2>& get_obstacles();
    int get_grid_width();
    int get_grid_height();
    int get_obstacle_amount();
    void set_robot_position(Vector2);
    void set_robot_orientation(int);
    void stop();
};

#endif
