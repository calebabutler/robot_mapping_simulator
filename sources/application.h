
// Begin header guard
#ifndef APPLICATION_H
#define APPLICATION_H

// Includes
#include <string>
#include "data_types.h"
#include "robot_server.h"
#include "plotter.h"

struct Parameters {
    int grid_width;
    int grid_height;
    int obstacle_amount;
    std::string algorithm;
};

struct Algorithm {
    std::string name;
    void (*sense)(RobotServer&);
    void (*plan)(RobotServer&);
    void (*act)(RobotServer&);
    void (*plot)(RobotServer&, Plotter&);
};

enum StepThroughType {
    FIRST_STEP,
    LAST_STEP,
    NO_MORE_STEPS,
    REGULAR_STEP,
};

class Application {
private:
    // Standard application data
    int m_grid_width;
    int m_grid_height;
    int m_obstacle_amount;
    std::string m_algorithm_name;
    // Robot position and orientation
    Vector2 m_robot_position;
    int m_robot_orientation;
    // Obstacle positions
    std::vector<Vector2> m_obstacles;
    std::vector<Vector2> m_found_obstacles;
    // Algorithms
    std::vector<Algorithm> m_algorithms;
    // Helper objects
    RobotServer m_server;
    Plotter m_plotter;
    // Additional data members
    StepThroughType m_step_type;
    int m_number_of_iterations;
    // Private member function for running the algorithm
    void process_parameters(const Parameters&);
    void generate_random_obstacles();
    void run_algorithm_once();
public:
    Application(const Parameters& parameters);
    void add_algorithm(std::string name, void (*sense)(RobotServer&),
                       void (*plan)(RobotServer&), void (*act)(RobotServer&),
                       void (*plot)(RobotServer&, Plotter&));
    void print_algorithms();
    void step_through();
    bool has_stopped();

    // Member function for Plotter
    void set_found_obstacles(const std::vector<Vector2>&);

    // Useful getters
    Vector2 get_robot_position();
    int get_robot_orientation();
    int get_number_of_iterations();
    const std::vector<Vector2>& get_obstacles();
    const std::vector<Vector2>& get_found_obstacles();
    int get_grid_width();
    int get_grid_height();
    int get_obstacle_amount();

    // Useful setters
    void set_robot_position(Vector2);
    void set_robot_orientation(int);

    // Member function for Robot sim server
    void stop();
};

#endif
