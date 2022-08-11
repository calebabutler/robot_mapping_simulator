
// Includes
#include "random_algorithm.h"
#include "robot_server.h"
#include "plotter.h"
#include <cstdlib>
#include <vector>

// Using namespace
using namespace std;

// Local function prototypes
void sense(RobotServer&);
void plan(RobotServer&);
void act(RobotServer&);
void plot(RobotServer&, Plotter&);

void add_random_algorithm(Application& app)
{
    app.add_algorithm("random", sense, plan, act, plot);
}

void sense(RobotServer& server)
{
}

void plan(RobotServer& server)
{
}

void act(RobotServer& server)
{
    // Generate random number between 0 and 2, store it in "move"
    int number = rand();
    double rand_max_plus_one = static_cast<double>(RAND_MAX) + 1.0;
    int move = number / rand_max_plus_one * 3;

    switch (move) {
    case 0:
        server.turn_left();
        break;
    case 1:
        server.move_forward();
        break;
    case 2:
        server.turn_right();
        break;
    }
}

void plot(RobotServer& server, Plotter& plotter)
{
    vector<Vector2> obstacles;

    int grid_width = server.get_grid_width();
    int grid_height = server.get_grid_height();

    for (int i = 0; i < grid_width; i++) {
        for (int j = 0; j < grid_height; j++) {
            obstacles.push_back(Vector2(i, j));
        }
    }

    plotter.plot(obstacles);
}
