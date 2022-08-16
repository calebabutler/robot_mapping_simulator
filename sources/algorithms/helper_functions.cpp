
// Includes
#include "helper_functions.h"

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

bool is_member(const std::vector<Vector2>& vector, Vector2 possible_element)
{
    bool found = false;
    for (int i = 0; i < vector.size() && !found; i++) {
        if (vector[i] == possible_element) {
            found = true;
        }
    }
    return found;
}

void perform_move(RobotServer& server, Move move)
{
    switch (move) {
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

void add_newly_found_obstacles(std::vector<Vector2>& obstacles,
                               const SensorData& sensor_data,
                               const Surroundings& surroundings)
{
    // Check if there are obstacles around the robot already known
    bool is_left_obstacle_known = is_member(obstacles, surroundings.left);
    bool is_front_obstacle_known = is_member(obstacles, surroundings.front);
    bool is_right_obstacle_known = is_member(obstacles, surroundings.right);

    // If the sensor picks up an obstacle not known, add it
    if (sensor_data.left && !is_left_obstacle_known) {
        obstacles.push_back(surroundings.left);
    }
    if (sensor_data.front && !is_front_obstacle_known) {
        obstacles.push_back(surroundings.front);
    }
    if (sensor_data.right && !is_right_obstacle_known) {
        obstacles.push_back(surroundings.right);
    }
}

Move number_to_move(int number)
{
    Move move;
    switch (number) {
    case 0:
        move = Move::TURN_LEFT;
        break;
    case 1:
        move = Move::TURN_RIGHT;
        break;
    case 2:
        move = Move::MOVE_FORWARD;
        break;
    default:
        move = Move::TURN_LEFT;
    }
    return move;
}
