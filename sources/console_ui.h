
// Begin header guard
#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

// Includes
#include "application.h"

class ConsoleUI {
private:
    Application m_app;
    void print_robot_and_obstacles();
public:
    ConsoleUI(const Parameters&);
    int run_loop();
};

// End header guard
#endif
