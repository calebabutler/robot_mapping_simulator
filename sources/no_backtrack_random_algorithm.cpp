
// Includes
#include "no_backtrack_random_algorithm.h"

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
static vector<Vector2> gl_found_obstacles;
static vector<Vector2> gl_previous_positions;
static Move gl_next_move;

// Function prototypes
static void sense(RobotServer&);
static Surroundings calculate_robot_surroundings(RobotServer&);
static void plan(RobotServer&);
static void act(RobotServer&);
static void plot(RobotServer&, Plotter&);

void add_no_backtrack_random_algorithm(Application& app)
{
    app.add_algorithm("no_backtrack_random", sense, plan, act, plot);
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

    // Add current position and orientation to the list of all positions and
    // orientations
    gl_previous_positions.push_back(server.get_position());

    // Stop server if you have found all obstacles
    if (gl_found_obstacles.size() == server.get_obstacle_amount()) {
        server.stop();
    }
}

void plan(RobotServer& server)
{
    Surroundings surroundings = calculate_robot_surroundings(server);
    Vector2 front = surroundings.front;
    int grid_width = server.get_grid_width();
    int grid_height = server.get_grid_height();

    bool is_left_free = true;
    bool is_front_free = true;
    bool is_right_free = true;

    // Check if obstacle is in front of the robot
    for (int i = 0; i < gl_found_obstacles.size() && is_front_free; i++) {
        if (gl_found_obstacles[i] == front) {
            is_front_free = false;
        }
    }
    // Check if the front of the robot would be outside the grid
    if (front.x < 0 || front.y < 0 || front.x >= grid_width || front.y >= grid_height) {
        is_front_free = false;
    }
    // Check if a move brings the robot to a previous position (not allowed)
    for (int i = 0; i < gl_previous_positions.size(); i++) {
        if (gl_previous_positions[i] == front) {
            is_front_free = false;
        }
    }

    // Generate random number between zero and RAND_MAX
    int number = rand();
    // Determine next move
    const Move moves[] = {Move::TURN_LEFT, Move::TURN_RIGHT, Move::MOVE_FORWARD};
    if (is_front_free) {
        gl_next_move = moves[number % 3];
    } else {
        gl_next_move = moves[number % 2];
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
