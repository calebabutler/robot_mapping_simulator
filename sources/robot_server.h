
// Begin header guard
#ifndef ROBOT_SERVER_H
#define ROBOT_SERVER_H

#include "data_types.h"

struct SensorData {
    bool left;
    bool front;
    bool right;
};

class Application;

class RobotServer {
private:
    Application& m_app;
public:
    // Constructor
    RobotServer(Application&);
    // Sensor read
    SensorData read_sensor();
    // Movement
    void turn_left();
    void move_forward();
    void turn_right();
    // Getters
    Vector2 get_position();
    int get_orientation();
    int get_number_of_iterations();
    int get_grid_width();
    int get_grid_height();
    int get_obstacle_amount();
    // Stop
    void stop();
};

// End header guard
#endif
