
// Begin header guard
#ifndef SFML_UI_H
#define SFML_UI_H

// Includes
#include "application.h"
#include <SFML/Graphics.hpp>

class SFMLUI {
private:
    Application m_app;
    sf::RenderWindow m_window;
    // Helper functions
    void draw_grid_lines();
    void draw_robot();
    void draw_obstacles();
public:
    SFMLUI(const Parameters&);
    int run_loop();
};

// End header guard
#endif
