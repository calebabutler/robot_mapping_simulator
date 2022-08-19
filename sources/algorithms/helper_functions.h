
// Begin header guard
#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

// Includes
#include "../data_types.h"
#include "../robot_server.h"

/// Helpful data structures ///

// This data structure is intended to hold the surrounding positions of the
// robot.
struct Surroundings {
    Vector2 left;
    Vector2 front;
    Vector2 right;
};

// This enum is useful for planning a move without performing it.
enum class Move {
    TURN_LEFT,
    MOVE_FORWARD,
    TURN_RIGHT,
};

/// Helpful functions ///

// This function calculates the left, front, and right of the robot.
Surroundings calculate_robot_surroundings(RobotServer&);

// This function does the same as the function above except it is given a
// position and orientation rather than a robot server.
Surroundings calculate_pose_surroundings(Vector2 position, int orientation);

// This function says if the Vector2 is a member of the array.
bool is_member(const std::vector<Vector2>&, Vector2);

// This function adds to the obstacles vector (first argument) the new obstacles
// sensed by the sensor data. The Surroundings data structure is used by the
// function to know to what positions the sensor data refers.
void add_newly_found_obstacles(std::vector<Vector2>&, const SensorData&, const Surroundings&);

// This function performs a move based on a move enum. Pseudocode:
// If move is TURN_LEFT, call server.turn_left()
// If move is MOVE_FORWARD, call server.move_forward()
// If move is TURN_RIGHT, call server.turn_right()
void perform_move(RobotServer&, Move);

// This function converts a number to a move enum, which is sometimes useful.
// Pseudocode:
// If the number is 0, return TURN_LEFT
// If the number is 1, return TURN_RIGHT
// If the number is 2, return MOVE_FORWARD
Move number_to_move(int);

// End header guard
#endif
