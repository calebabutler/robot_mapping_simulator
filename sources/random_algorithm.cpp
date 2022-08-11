
// Includes
#include "random_algorithm.h"
#include "robot_server.h"
#include "plotter.h"
#include <cstdlib>
#include <vector>

// Using namespace
using namespace std;

// Local data types
struct Surroundings {
    Vector2 left;
    Vector2 front;
    Vector2 right;
};

enum class Move {
    TURN_LEFT,
    MOVE_FORWARD,
    TURN_RIGHT,
};

// Globals (local to this file)
vector<Vector2> gl_found_obstacles;
Move gl_next_move;

// Local function prototypes
void sense(RobotServer&);
Surroundings calculate_robot_surroundings(RobotServer&);
void plan(RobotServer&);
void act(RobotServer&);
void plot(RobotServer&, Plotter&);

void add_random_algorithm(Application& app)
{
    app.add_algorithm("random", sense, plan, act, plot);
}

void sense(RobotServer& server)
{
    SensorData data = server.read_sensor();
    Surroundings surroundings = calculate_robot_surroundings(server);

    // Check if there are obstacles around the robot already known
    bool is_left_obstacle = false;
    bool is_front_obstacle = false;
    bool is_right_obstacle = false;

    for (int i = 0; i < gl_found_obstacles.size(); i++) {
        if (gl_found_obstacles[i] == surroundings.left) {
            is_left_obstacle = true;
        }
        if (gl_found_obstacles[i] == surroundings.front) {
            is_front_obstacle = true;
        }
        if (gl_found_obstacles[i] == surroundings.right) {
            is_right_obstacle = true;
        }
    }

    // If the sensor picks up an obstacle not known, add it
    if (data.left && !is_left_obstacle) {
        gl_found_obstacles.push_back(surroundings.left);
    }
    if (data.front && !is_front_obstacle) {
        gl_found_obstacles.push_back(surroundings.front);
    }
    if (data.right && !is_right_obstacle) {
        gl_found_obstacles.push_back(surroundings.right);
    }

    // Stop server if you have found all obstacles
    if (gl_found_obstacles.size() == server.get_obstacle_amount()) {
        server.stop();
    }
}

Surroundings calculate_robot_surroundings(RobotServer& server)
{
    Surroundings surroundings;

    Vector2 position = server.get_position();

    Vector2 overhead_top = position + Vector2(0, 1);
    Vector2 overhead_left = position + Vector2(-1, 0);
    Vector2 overhead_right = position + Vector2(1, 0);
    Vector2 overhead_bottom = position + Vector2(0, -1);

    switch (server.get_orientation()) {
    case 0:
        surroundings.left = overhead_left;
        surroundings.front = overhead_top;
        surroundings.right = overhead_right;
        break;
    case 1:
        surroundings.left = overhead_bottom;
        surroundings.front = overhead_left;
        surroundings.right = overhead_top;
        break;
    case 2:
        surroundings.left = overhead_right;
        surroundings.front = overhead_bottom;
        surroundings.right = overhead_left;
        break;
    case 3:
        surroundings.left = overhead_top;
        surroundings.front = overhead_right;
        surroundings.right = overhead_bottom;
        break;
    }
    return surroundings;
}

void plan(RobotServer& server)
{
    Surroundings surroundings = calculate_robot_surroundings(server);
    Vector2 front = surroundings.front;
    int grid_width = server.get_grid_width();
    int grid_height = server.get_grid_height();

    bool is_obstacle_in_front = false;
    bool is_front_out_of_grid = false;

    // Check if obstacle is in front of the robot
    for (int i = 0; i < gl_found_obstacles.size() && !is_obstacle_in_front; i++) {
        if (gl_found_obstacles[i] == front) {
            is_obstacle_in_front = true;
        }
    }
    // Check if the front of the robot would be outside the grid
    if (front.x < 0 || front.y < 0 || front.x >= grid_width || front.y >= grid_height) {
        is_front_out_of_grid = true;
    }

    // Generate random number between zero and RAND_MAX
    int number = rand();

    if (is_obstacle_in_front || is_front_out_of_grid) { // If you cannot move forward
        // Turn the random number into either 0 or 1
        int zero_or_one = number % 2;
        switch (zero_or_one) {
        case 0:
            gl_next_move = Move::TURN_LEFT;
            break;
        case 1:
            gl_next_move = Move::TURN_RIGHT;
            break;
        }
    } else {
        // Turn the random number into 0, 1, or 2
        int zero_one_or_two = number % 3;
        switch (zero_one_or_two) {
        case 0:
            gl_next_move = Move::TURN_LEFT;
            break;
        case 1:
            gl_next_move = Move::MOVE_FORWARD;
            break;
        case 2:
            gl_next_move = Move::TURN_RIGHT;
            break;
        }
    }
}

void act(RobotServer& server)
{
    switch (gl_next_move) {
    case Move::TURN_LEFT:
        server.turn_left();
        break;
    case Move::MOVE_FORWARD:
        server.move_forward();
        break;
    case Move::TURN_RIGHT:
        server.turn_right();
        break;
    }
}

void plot(RobotServer& server, Plotter& plotter)
{
    plotter.plot(gl_found_obstacles);
}
