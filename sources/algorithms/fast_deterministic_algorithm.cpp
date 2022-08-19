
// Includes
#include "fast_deterministic_algorithm.h"
#include "helper_functions.h"
#include <climits>
#include <iostream>
#include <stack>
#include <vector>

// Using namespace
using namespace std;

// Local types
struct Pose {
    Vector2 position;
    int orientation;
};

struct AStarInfo {
    Pose current_node;
    int F;
    int G;
    int H;
    Pose parent_node;
};

// Globals (local to this file)
static vector<Vector2> gl_found_obstacles;
static vector<Vector2> gl_previously_seen_spaces;
static Move gl_next_move;
static stack<Move> gl_move_list;
static int gl_old_obstacle_amount;

// Local function prototypes
static void sense(RobotServer&);
static void plan(RobotServer&);
static void act(RobotServer&);
static void plot(RobotServer&, Plotter&);
static Pose calculate_next_pose(RobotServer&);
static stack<Move> calculate_best_path(RobotServer&, Pose, Pose);
static int calculate_distance(Vector2, Vector2);
static bool specialized_is_member(const vector<AStarInfo>&, Pose);

void add_fast_deterministic_algorithm(Application& app)
{
    // Initialize globals
    gl_old_obstacle_amount = 0;

    // Add algorithm
    app.add_algorithm("fast_deterministic", sense, plan, act, plot);
}

void sense(RobotServer& server)
{
    SensorData data = server.read_sensor();
    Surroundings surroundings = calculate_robot_surroundings(server);

    add_newly_found_obstacles(gl_found_obstacles, data, surroundings);

    // Add to previously seen positions
    if (!is_member(gl_previously_seen_spaces, surroundings.front)) {
        gl_previously_seen_spaces.push_back(surroundings.front);
    }
    if (!is_member(gl_previously_seen_spaces, surroundings.left)) {
        gl_previously_seen_spaces.push_back(surroundings.left);
    }
    if (!is_member(gl_previously_seen_spaces, surroundings.right)) {
        gl_previously_seen_spaces.push_back(surroundings.right);
    }
    if (!is_member(gl_previously_seen_spaces, server.get_position())) {
        gl_previously_seen_spaces.push_back(server.get_position());
    }

    // Stop server if you have found all obstacles
    if (gl_found_obstacles.size() == server.get_obstacle_amount()) {
        server.stop();
    }
}

void plan(RobotServer& server)
{
    if (gl_move_list.empty() || gl_found_obstacles.size() > gl_old_obstacle_amount) {
        Pose next_pose = calculate_next_pose(server);

        Pose current_pose;
        current_pose.position = server.get_position();
        current_pose.orientation = server.get_orientation();

        gl_move_list = calculate_best_path(server, current_pose, next_pose);
        gl_old_obstacle_amount = gl_found_obstacles.size();
    }

    // If the newly generated path is empty, stop the simulation
    if (gl_move_list.empty()) {
        server.stop();
    } else {
        gl_next_move = gl_move_list.top();
        gl_move_list.pop();
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

Pose calculate_next_pose(RobotServer& server)
{
    Pose pose = {{0, 0}, 0};
    int minimum_distance = INT_MAX;
    int maximum_information_value = INT_MIN;

    for (int x = 0; x < server.get_grid_width(); x++) {
        for (int y = 0; y < server.get_grid_height(); y++) {
            for (int orientation = 0; orientation < 4; orientation++) {
                Pose p = {{x, y}, orientation};
                int information_value;

                if (is_member(gl_found_obstacles, p.position)) {
                    information_value = 0;
                } else {
                    Surroundings p_surroundings = calculate_pose_surroundings(p.position, p.orientation);
                    information_value = 3;
                    if (is_member(gl_previously_seen_spaces, p_surroundings.left)) {
                        information_value--;
                    }
                    if (is_member(gl_previously_seen_spaces, p_surroundings.front)) {
                        information_value--;
                    }
                    if (is_member(gl_previously_seen_spaces, p_surroundings.right)) {
                        information_value--;
                    }
                }

                int distance = calculate_distance(server.get_position(), p.position);

                if (information_value > maximum_information_value ||
                        (information_value == maximum_information_value && distance < minimum_distance)) {
                    pose = p;
                    maximum_information_value = information_value;
                    minimum_distance = distance;
                }
            }
        }
    }

    if (maximum_information_value <= 0) {
        server.stop();
    }

    return pose;
}

int calculate_distance(Vector2 v1, Vector2 v2)
{
    // Uses manhattan distance
    return abs(v1.x - v2.x) + abs(v1.y - v2.y);
}

stack<Move> calculate_best_path(RobotServer& server, Pose start, Pose end)
{
    // FIXME: vector is not the right data structure for open list since
    // random elements need to be deleted at any time
    vector<AStarInfo> open_list;

    vector<AStarInfo> closed_list;

    // Add start to open list
    int H = calculate_distance(start.position, end.position);
    AStarInfo start_info = {start, H, 0, H, {{-1, -1}, 0}};
    open_list.push_back(start_info);

    // Add obstacles to closed list
    for (int i = 0; i < gl_found_obstacles.size(); i++) {
        for (int orientation = 0; orientation < 4; orientation++) {
            Pose pose = {gl_found_obstacles[i], orientation};
            AStarInfo info = {pose, 0, 0, 0, {{-1, -1}, 0}};
            closed_list.push_back(info);
        }
    }

    bool has_failed = false;

    while (!has_failed && !specialized_is_member(open_list, end)) {
        if (open_list.empty()) {
            has_failed = true;
        } else {
            // Choose the node in the open list with the smallest F value
            int minimum_F = INT_MAX;
            int chosen_node_index = 0;
            for (int i = 0; i < open_list.size(); i++) {
                if (open_list[i].F < minimum_F) {
                    minimum_F = open_list[i].F;
                    chosen_node_index = i;
                }
            }
            // Expand the chosen node into three nodes
            int G = open_list[chosen_node_index].G + 1;
            Pose old_pose = open_list[chosen_node_index].current_node;
            Surroundings surroundings = calculate_pose_surroundings(old_pose.position, old_pose.orientation);

            Pose next_nodes[3] = {
                {old_pose.position, (old_pose.orientation + 1) % 4}, // Left turn
                {surroundings.front, old_pose.orientation}, // Move forward
                {old_pose.position, (old_pose.orientation + 3) % 4}, // Right turn
            };

            for (int i = 0; i < 3; i++) {
                Vector2 pos = next_nodes[i].position;

                if (pos.x >= 0 && pos.x < server.get_grid_width() &&
                    pos.y >= 0 && pos.y < server.get_grid_height() &&
                    !is_member(gl_found_obstacles, pos)) {

                    int H = calculate_distance(pos, end.position);
                    AStarInfo info = {next_nodes[i], G + H, G, H, old_pose};
                    open_list.push_back(info);
                }
            }
            // Put chosen node into closed list
            closed_list.push_back(open_list[chosen_node_index]);
            // Remove chosen node from open list
            open_list.erase(open_list.begin() + chosen_node_index);
        }
    }

    stack<Move> move_list;

    // If the algorithm never reached end node, return empty move list.
    // Otherwise,
    if (!has_failed) { 
        // Find node info with end pose
        AStarInfo* node_info = nullptr;
        bool found = false;
        for (int i = 0; i < open_list.size() && !found; i++) {
            if (open_list[i].current_node.position == end.position && open_list[i].current_node.orientation == end.orientation) {
                node_info = &open_list[i];
                found = true;
            }
        }

        // Keep adding moves to the move list until node info has start pose
        while (node_info->current_node.position != start.position || node_info->current_node.orientation != start.orientation) {
            if (node_info->current_node.position != node_info->parent_node.position) {
                move_list.push(Move::MOVE_FORWARD);
            } else if (node_info->current_node.orientation == (node_info->parent_node.orientation + 1) % 4) {
                move_list.push(Move::TURN_LEFT);
            } else {
                move_list.push(Move::TURN_RIGHT);
            }

            // Find parent node in closed list
            found = false;
            for (int i = 0; i < closed_list.size() && !found; i++) {
                if (closed_list[i].current_node.position == node_info->parent_node.position && closed_list[i].current_node.orientation == node_info->parent_node.orientation) {
                    node_info = &closed_list[i];
                    found = true;
                }
            }
        }
    }

    return move_list;
}

bool specialized_is_member(const vector<AStarInfo>& list, Pose pose)
{
    bool found = false;
    for (int i = 0; i < list.size() && !found; i++) {
        Pose this_pose = list[i].current_node;
        if (this_pose.position == pose.position && this_pose.orientation == pose.orientation) {
            found = true;
        }
    }
    return found;
}
