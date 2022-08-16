
// Includes
#include "random_algorithm.h"
#include "helper_functions.h"
#include <cstdlib>
#include <vector>

// Using namespace
using namespace std;

// Globals (local to this file)
static vector<Vector2> gl_found_obstacles;
static Move gl_next_move;

// Local function prototypes
static void sense(RobotServer&);
static void plan(RobotServer&);
static void act(RobotServer&);
static void plot(RobotServer&, Plotter&);

void add_random_algorithm(Application& app)
{
    app.add_algorithm("random", sense, plan, act, plot);
}

void sense(RobotServer& server)
{
    SensorData data = server.read_sensor();
    Surroundings surroundings = calculate_robot_surroundings(server);

    add_newly_found_obstacles(gl_found_obstacles, data, surroundings);

    // Stop server if you have found all obstacles
    if (gl_found_obstacles.size() == server.get_obstacle_amount()) {
        server.stop();
    }
}

void plan(RobotServer& server)
{
    Surroundings surroundings = calculate_robot_surroundings(server);

    // Small variable names to make things more readable
    Vector2 front = surroundings.front;
    int grid_width = server.get_grid_width();
    int grid_height = server.get_grid_height();

    // Check if obstacle is in front of the robot
    bool is_obstacle_in_front = is_member(gl_found_obstacles, front);

    // Check if the front of the robot would be outside the grid
    bool is_front_out_of_grid;
    if (front.x < 0 || front.y < 0 || front.x >= grid_width || front.y >= grid_height) {
        is_front_out_of_grid = true;
    } else {
        is_front_out_of_grid = false;
    }

    // Generate random number between zero and RAND_MAX
    int number = rand();

    if (is_obstacle_in_front || is_front_out_of_grid) { // If you cannot move forward
        // Only turn the robot either left or right
        gl_next_move = number_to_move(number % 2);
    } else {
        // Make the robot turn left, turn right, or move forward
        gl_next_move = number_to_move(number % 3);
    }
}

void act(RobotServer& server)
{
    perform_move(server, gl_next_move);
}

void plot(RobotServer& server, Plotter& plotter)
{
    plotter.plot(gl_found_obstacles);
}
