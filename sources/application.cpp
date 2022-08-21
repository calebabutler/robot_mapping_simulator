
// Includes
#include "application.h"
#include "algorithms/algorithms.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Application::Application(const Parameters& parameters) : m_server(*this), m_plotter(*this)
{
    // Add algorithms
    add_algorithms(*this);

    process_parameters(parameters);

    m_robot_position.x = 0;
    m_robot_position.y = 0;
    m_robot_orientation = 1;
    m_number_of_iterations = 0;
}

void Application::process_parameters(const Parameters& parameters)
{
    m_grid_width = parameters.grid_width;
    m_grid_height = parameters.grid_height;
    m_obstacle_amount = parameters.obstacle_amount;
    m_algorithm_name = parameters.algorithm;

    m_step_type = StepThroughType::NO_MORE_STEPS;

    bool is_algorithm_in_algorithms = false;

    for (int i = 0; i < m_algorithms.size() && !is_algorithm_in_algorithms; i++) {
        if (m_algorithms[i].name == m_algorithm_name) {
            is_algorithm_in_algorithms = true;
        }
    }

    if (m_grid_width < 1) {
        std::cerr << "Grid width is too small." << std::endl;
    } else if (m_grid_height < 1) {
        std::cerr << "Grid height is too small." << std::endl;
    } else if (m_obstacle_amount < 1) {
        std::cerr << "Obstacle amount is too small." << std::endl;
    } else if (m_obstacle_amount >= m_grid_width * m_grid_height) {
        std::cerr << "Obstacle amount is too big." << std::endl;
    } else if (!is_algorithm_in_algorithms) {
        std::cerr << "That algorithm is not available." << std::endl;
    } else {
        m_step_type = StepThroughType::FIRST_STEP;
    }
}

void Application::add_algorithm(std::string name, void (*sense)(RobotServer&),
                                void (*plan)(RobotServer&),
                                void (*act)(RobotServer&),
                                void (*plot)(RobotServer&, Plotter&))
{
    Algorithm alg = {name, sense, plan, act, plot};
    m_algorithms.push_back(alg);
}

void Application::print_algorithms()
{
    for (const Algorithm& alg : m_algorithms) {
        std::cout << alg.name << std::endl;
    }
}

void Application::step_through()
{
    switch (m_step_type) {
    case StepThroughType::FIRST_STEP:
        generate_random_obstacles();
        m_step_type = StepThroughType::REGULAR_STEP;
    case StepThroughType::LAST_STEP:
    case StepThroughType::REGULAR_STEP:
        run_algorithm_once();
        break;
    case StepThroughType::NO_MORE_STEPS:
        break;
    }
}

void Application::generate_random_obstacles()
{
    m_obstacles.reserve(m_obstacle_amount);
    // Seed random number generator
    std::srand(std::time(nullptr));
    // Generate random obstacles
    for (int i = 0; i < m_obstacle_amount; i++) {
        int x = 0;
        int y = 0;
        // Do not put an obstacle at the origin
        while (x == 0 && y == 0) {
            x = std::rand() % m_grid_width;
            y = std::rand() % m_grid_height;
            // Make sure x and y are not already an obstacle
            for (int i = 0; i < m_obstacles.size() && !(x == 0 && y == 0); i++) {
                if (Vector2(x, y) == m_obstacles[i]) {
                    x = 0;
                    y = 0;
                }
            }
        }
        m_obstacles.push_back(Vector2(x, y));
    }
}

void Application::run_algorithm_once()
{
    int alg_index;
    bool found_alg = false;

    // Search for alg
    for (int i = 0; i < m_algorithms.size() && !found_alg; i++) {
        if (m_algorithms[i].name == m_algorithm_name) {
            alg_index = i;
            found_alg = true;
        }
    }

    if (m_algorithms[alg_index].sense != nullptr && m_step_type != LAST_STEP) {
        m_algorithms[alg_index].sense(m_server);
    }
    if (m_algorithms[alg_index].plan != nullptr && m_step_type != LAST_STEP) {
        m_algorithms[alg_index].plan(m_server);
    }
    if (m_algorithms[alg_index].act != nullptr && m_step_type != LAST_STEP) {
        m_algorithms[alg_index].act(m_server);
    }
    if (m_algorithms[alg_index].plot != nullptr) {
        m_algorithms[alg_index].plot(m_server, m_plotter);
    }

    // Add one to the number of iterations
    m_number_of_iterations++;

    // Print number of iterations if last step
    if (m_step_type == LAST_STEP) {
        std::cout << std::endl;
        std::cout << "Number of iterations: " << m_number_of_iterations << std::endl;
        m_step_type = NO_MORE_STEPS;
    }
}

void Application::set_found_obstacles(const std::vector<Vector2>& obstacles)
{
    m_found_obstacles = obstacles;
}

Vector2 Application::get_robot_position()
{
    return m_robot_position;
}

int Application::get_robot_orientation()
{
    return m_robot_orientation;
}

int Application::get_number_of_iterations()
{
    return m_number_of_iterations;
}

const std::vector<Vector2>& Application::get_obstacles()
{
    return m_obstacles;
}

const std::vector<Vector2>& Application::get_found_obstacles()
{
    return m_found_obstacles;
}

int Application::get_grid_width()
{
    return m_grid_width;
}

int Application::get_grid_height()
{
    return m_grid_height;
}

int Application::get_obstacle_amount()
{
    return m_obstacle_amount;
}

void Application::set_robot_position(Vector2 position)
{
    m_robot_position = position;
}

void Application::set_robot_orientation(int orientation)
{
    m_robot_orientation = orientation;
}

void Application::stop()
{
    m_step_type = StepThroughType::LAST_STEP;
}
