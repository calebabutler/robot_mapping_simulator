
// Includes
#include "plotter.h"
#include "application.h"

Plotter::Plotter(Application& a) : m_app(a)
{
}

void Plotter::plot(const std::vector<Vector2>& obstacles)
{
    m_app.set_found_obstacles(obstacles);
}
