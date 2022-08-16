
// Includes
#include "application.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>

// Global constants (defaults)
const int DEFAULT_WINDOW_WIDTH = 500;
const int DEFAULT_WINDOW_HEIGHT = 500;
const int DEFAULT_GRID_WIDTH = 4;
const int DEFAULT_GRID_HEIGHT = 4;
const int DEFAULT_OBSTACLE_AMOUNT = 4;
const std::string DEFAULT_ALGORITHM_NAME = "random";
const ApplicationMode DEFAULT_MODE = ApplicationMode::HELP;

// Local function prototypes
void print_info(int w_width, int w_height, int g_width, int g_height,
                int obstacles, std::string algorithm, ApplicationMode mode);

Application::Application()
{
    m_window_width = DEFAULT_WINDOW_WIDTH;
    m_window_height = DEFAULT_WINDOW_HEIGHT;
    m_grid_width = DEFAULT_GRID_WIDTH;
    m_grid_height = DEFAULT_GRID_HEIGHT;
    m_obstacle_amount = DEFAULT_OBSTACLE_AMOUNT;
    m_algorithm_name = DEFAULT_ALGORITHM_NAME;
    m_mode = DEFAULT_MODE;

    m_robot_position.x = 0;
    m_robot_position.y = 0;
    m_robot_orientation = 1;

    m_number_of_iterations = 0;
    m_keep_running = true;
}

void Application::add_algorithm(std::string name, void (*sense)(RobotServer&),
                                void (*plan)(RobotServer&),
                                void (*act)(RobotServer&),
                                void (*plot)(RobotServer&, Plotter&))
{
    Algorithm alg = {name, sense, plan, act, plot};
    m_algorithms.push_back(alg);
}

void Application::set_window_width(int width)
{
    m_window_width = width;
}

void Application::set_window_height(int height)
{
    m_window_height = height;
}

void Application::set_grid_width(int width)
{
    m_grid_width = width;
}

void Application::set_grid_height(int height)
{
    m_grid_height = height;
}

void Application::set_obstacle_amount(int amount)
{
    m_obstacle_amount = amount;
}

void Application::set_algorithm(std::string name)
{
    m_algorithm_name = name;
}

void Application::set_mode(ApplicationMode mode)
{
    m_mode = mode;
}

int Application::run()
{
    int error_code = 0;

    switch (m_mode) {
    case ApplicationMode::AUTO:
    case ApplicationMode::STEP_THROUGH:
        run_simulation();
        break;
    case ApplicationMode::HELP:
        print_help();
        break;
    case ApplicationMode::DEFAULTS:
        print_defaults();
        break;
    case ApplicationMode::ALGORITHMS:
        print_algorithms();
        break;
    }

    return error_code;
}

void Application::print_help()
{
    std::cout << "List of options:" << std::endl;
    std::cout << "  -help                   Print this help message" << std::endl;
    std::cout << "  -auto                   Run the simulation on auto mode" << std::endl;
    std::cout << "  -step-through           Run the simulation requiring a key press before every move" << std::endl;
    std::cout << "  -list-defaults          List the default settings for the simulation" << std::endl;
    std::cout << "  -list-algorithms        List the available mapping algorithms" << std::endl;
    std::cout << "  -window-width [int]     Change the window width" << std::endl;
    std::cout << "  -window-height [int]    Change the window height" << std::endl;
    std::cout << "  -grid-width [int]       Change the grid width" << std::endl;
    std::cout << "  -grid-height [int]      Change the grid height" << std::endl;
    std::cout << "  -obstacle-amount [int]  Change the amount of obstacles" << std::endl;
    std::cout << "  -algorithm [string]     Change the algorithm used" << std::endl;
}

void print_info(int w_width, int w_height, int g_width, int g_height,
                int obstacles, std::string algorithm, ApplicationMode mode)
{
    std::cout << "window width:    " << w_width << std::endl;
    std::cout << "window height:   " << w_height << std::endl;
    std::cout << "grid width:      " << g_width << std::endl;
    std::cout << "grid height:     " << g_height << std::endl;
    std::cout << "obstacle amount: " << obstacles << std::endl;
    std::cout << "algorithm:       " << algorithm << std::endl;
    std::cout << "mode:            ";
    switch (mode) {
    case ApplicationMode::AUTO:
        std::cout << "auto" << std::endl;
        break;
    case ApplicationMode::STEP_THROUGH:
        std::cout << "step through" << std::endl;
        break;
    case ApplicationMode::HELP:
        std::cout << "help" << std::endl;
        break;
    case ApplicationMode::DEFAULTS:
        std::cout << "list defaults" << std::endl;
        break;
    case ApplicationMode::ALGORITHMS:
        std::cout << "list algorithms" << std::endl;
        break;
    }
}

void Application::print_defaults()
{
    print_info(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_GRID_WIDTH,
               DEFAULT_GRID_HEIGHT, DEFAULT_OBSTACLE_AMOUNT,
               DEFAULT_ALGORITHM_NAME, DEFAULT_MODE);
}

void Application::print_algorithms()
{
    for (const Algorithm& alg : m_algorithms) {
        std::cout << alg.name << std::endl;
    }
}

void Application::run_simulation()
{
    // Print simulation information to console
    print_info(m_window_width, m_window_height, m_grid_width, m_grid_height,
               m_obstacle_amount, m_algorithm_name, m_mode);

    // Generate random obstacles
    generate_random_obstacles();

    // Create robot server and plotter
    RobotServer server(*this);
    Plotter plotter(*this);

    // Create window
    sf::RenderWindow window(sf::VideoMode(m_window_width, m_window_height), "Robot Mapping Simulator");

    bool is_key_pressed = false;
    bool has_iterations_printed = false;

    // Run main loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                is_key_pressed = true;
            } else {
                is_key_pressed = false;
            }
        }
        // Run through the mapping algorithm one step
        if (m_keep_running) {
            if (m_mode == ApplicationMode::AUTO || is_key_pressed) {
                run_algorithm_once(server, plotter);
                sf::sleep(sf::milliseconds(100));
            }
        // Print number of iterations if the mapping algorithm is done
        } else if (!has_iterations_printed) {
            std::cout << std::endl;
            std::cout << "Number of iterations: " << m_number_of_iterations << std::endl;
            has_iterations_printed = true;
        }
        // Draw everything
        window.clear(sf::Color(170, 170, 170));
        draw_grid_lines(window);
        draw_robot(window);
        draw_obstacles(window);
        window.display();
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

void Application::run_algorithm_once(RobotServer& server, Plotter& plotter)
{
    int alg_index = -1;
    bool found_alg = false;

    for (int i = 0; i < m_algorithms.size() && !found_alg; i++) {
        if (m_algorithms[i].name == m_algorithm_name) {
            alg_index = i;
            found_alg = true;
        }
    }

    if (alg_index == -1) {
        std::cout << "Invalid algorithm name" << std::endl;
        exit(-1);
    }

    if (m_algorithms[alg_index].sense != nullptr && m_keep_running) {
        m_algorithms[alg_index].sense(server);
    }
    if (m_algorithms[alg_index].plan != nullptr && m_keep_running) {
        m_algorithms[alg_index].plan(server);
    }
    if (m_algorithms[alg_index].act != nullptr && m_keep_running) {
        m_algorithms[alg_index].act(server);
    }
    if (m_algorithms[alg_index].plot != nullptr) {
        m_algorithms[alg_index].plot(server, plotter);
    }

    // Add one to the number of iterations
    m_number_of_iterations++;
}

void Application::draw_grid_lines(sf::RenderWindow& window)
{
    // Create vector of vertices, every two verticies is a single grid line
    std::vector<sf::Vertex> lines;
    lines.reserve((m_grid_width + m_grid_height) * 2);

    // Populate lines vector
    for (int i = 1; i < m_grid_width; i++) {
        float x = static_cast<float>(m_window_width) / m_grid_width * i;
        lines.push_back(sf::Vertex(sf::Vector2f(x, 0)));
        lines.push_back(sf::Vertex(sf::Vector2f(x, m_window_height)));
    }
    for (int i = 1; i < m_grid_height; i++) {
        float y = static_cast<float>(m_window_height) / m_grid_height * i;
        lines.push_back(sf::Vertex(sf::Vector2f(0, y)));
        lines.push_back(sf::Vertex(sf::Vector2f(m_window_width, y)));
    }

    // Set color of all grid lines
    for (sf::Vertex& vertex : lines) {
        vertex.color = sf::Color(70, 70, 70);
    }

    // Draw grid lines
    window.draw(lines.data(), lines.size(), sf::Lines);
}

void Application::draw_robot(sf::RenderWindow& window)
{
    // Find radius
    float cell_width = static_cast<float>(m_window_width) / m_grid_width;
    float cell_height = static_cast<float>(m_window_height) / m_grid_height;
    float radius = std::min(cell_width / 2, cell_height / 2) - 20;

    // Find x and y values
    float x = (m_robot_position.x + 0.5) * cell_width;
    float y = m_window_height - (m_robot_position.y + 0.5) * cell_height;

    // Draw triangle
    sf::CircleShape robot(radius, 3);
    robot.setPosition(x, y);
    robot.setOrigin(radius, radius);
    robot.setRotation(m_robot_orientation * -90);
    robot.setFillColor(sf::Color(70, 70, 170));
    window.draw(robot);
}

void Application::draw_obstacles(sf::RenderWindow& window)
{
    // Find radius
    float cell_width = static_cast<float>(m_window_width) / m_grid_width;
    float cell_height = static_cast<float>(m_window_height) / m_grid_height;
    float radius = std::min(cell_width / 2, cell_height / 2) - 20;

    for (Vector2& obstacle : m_found_obstacles) {
        // Find x and y values
        float x = (obstacle.x + 0.5) * cell_width;
        float y = m_window_height - (obstacle.y + 0.5) * cell_height;

        sf::CircleShape obstacle_shape(radius, 4);
        obstacle_shape.setOrigin(radius, radius);
        obstacle_shape.setPosition(x, y);
        obstacle_shape.setFillColor(sf::Color(170, 70, 70));
        window.draw(obstacle_shape);
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
    m_keep_running = false;
}
