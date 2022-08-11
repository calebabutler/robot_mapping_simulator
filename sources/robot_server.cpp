
// Includes
#include "robot_server.h"
#include "application.h"

RobotServer::RobotServer(Application& a) : m_app(a)
{
}

SensorData RobotServer::read_sensor()
{
    SensorData data = {false, false, false};

    Vector2 position = m_app.get_robot_position();
    int orientation = m_app.get_robot_orientation();

    Vector2 absolute_top = position + Vector2(0, 1);
    Vector2 absolute_left = position + Vector2(-1, 0);
    Vector2 absolute_right = position + Vector2(1, 0);
    Vector2 absolute_bottom = position + Vector2(0, -1);

    Vector2 left, front, right;

    switch (orientation) {
    case 0:
        left = absolute_left;
        front = absolute_top;
        right = absolute_right;
        break;
    case 1:
        left = absolute_bottom;
        front = absolute_left;
        right = absolute_top;
        break;
    case 2:
        left = absolute_right;
        front = absolute_bottom;
        right = absolute_left;
        break;
    case 3:
        left = absolute_top;
        front = absolute_right;
        right = absolute_bottom;
        break;
    }

    const std::vector<Vector2>& obstacles = m_app.get_obstacles();

    for (Vector2 obstacle : obstacles) {
        if (obstacle == left) {
            data.left = true;
        } else if (obstacle == front) {
            data.front = true;
        } else if (obstacle == right) {
            data.right = true;
        }
    }

    return data;
}

void RobotServer::turn_left()
{
    int orientation = m_app.get_robot_orientation();
    m_app.set_robot_orientation((orientation + 1) % 4);
}

void RobotServer::move_forward()
{
    Vector2 position = m_app.get_robot_position();
    int orientation = m_app.get_robot_orientation();

    switch (orientation) {
    case 0:
        m_app.set_robot_position(position + Vector2(0, 1));
        break;
    case 1:
        m_app.set_robot_position(position + Vector2(-1, 0));
        break;
    case 2:
        m_app.set_robot_position(position + Vector2(0, -1));
        break;
    case 3:
        m_app.set_robot_position(position + Vector2(1, 0));
        break;
    }
}

void RobotServer::turn_right()
{
    int orientation = m_app.get_robot_orientation();
    m_app.set_robot_orientation((orientation + 3) % 4);
}

Vector2 RobotServer::get_position()
{
    return m_app.get_robot_position();
}

int RobotServer::get_orientation()
{
    return m_app.get_robot_orientation();
}

int RobotServer::get_number_of_iterations()
{
    return m_app.get_number_of_iterations();
}

int RobotServer::get_grid_width()
{
    return m_app.get_grid_width();
}

int RobotServer::get_grid_height()
{
    return m_app.get_grid_height();
}

int RobotServer::get_obstacle_amount()
{
    return m_app.get_obstacle_amount();
}

void RobotServer::stop()
{
    m_app.stop();
}
