
// Includes
#include "console_ui.h"
#include <iostream>

ConsoleUI::ConsoleUI(const Parameters& parameters) : m_app(parameters)
{
}

int ConsoleUI::run_loop()
{
    while (!m_app.has_stopped()) {
        m_app.step_through();
        print_robot_and_obstacles();
    }
    return 0;
}

void ConsoleUI::print_robot_and_obstacles()
{
    using std::cout;
    using std::endl;
    using std::vector;

    vector<vector<char>> grid;

    for (int i = 0; i < m_app.get_grid_width(); i++) {
        grid.push_back(vector<char>());
        for (int j = 0; j < m_app.get_grid_height(); j++) {
            grid[i].push_back(' ');
        }
    }

    Vector2 position = m_app.get_robot_position();
    switch (m_app.get_robot_orientation()) {
    case 0:
        grid[position.x][position.y] = '^';
        break;
    case 1:
        grid[position.x][position.y] = '<';
        break;
    case 2:
        grid[position.x][position.y] = 'v';
        break;
    case 3:
        grid[position.x][position.y] = '>';
        break;
    }

    const vector<Vector2>& obstacles = m_app.get_found_obstacles();
    for (int i = 0; i < obstacles.size(); i++) {
        grid[obstacles[i].x][obstacles[i].y] = '*';
    }

    for (int i = 0; i < m_app.get_grid_width() + 2; i++) {
        cout << '-';
    }
    cout << endl;

    for (int i = 0; i < m_app.get_grid_height(); i++) {
        cout << '|';
        for (int j = 0; j < m_app.get_grid_width(); j++) {
            cout << grid[j][m_app.get_grid_height() - i - 1];
        }
        cout << '|';
        cout << endl;
    }

    for (int i = 0; i < m_app.get_grid_width() + 2; i++) {
        cout << '-';
    }
    cout << endl;
}
