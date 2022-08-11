
// Begin header guard
#ifndef PLOTTER_H
#define PLOTTER_H

// Includes
#include "data_types.h"

class Application;

class Plotter {
private:
    Application& m_app;
public:
    Plotter(Application&);
    void plot(const std::vector<Vector2>& obstacles);
};

// End header guard
#endif
