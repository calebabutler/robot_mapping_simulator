
// Includes
#include "algorithms.h"
#include "application.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

// Local types
enum class LongOptionWithArgument {
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    GRID_WIDTH,
    GRID_HEIGHT,
    OBSTACLE_AMOUNT,
    ALGORITHM,
};

// Function prototypes
void parse_arguments(int argc, char* argv[], Application& app);
int convert_string_to_int(char*);

int main(int argc, char* argv[])
{
    Application app;

    add_algorithms(app);
    parse_arguments(argc, argv, app);

    return app.run();
}

void parse_arguments(int argc, char* argv[], Application& app)
{
    LongOptionWithArgument last_option;
    bool is_argument = false;

    for (int i = 1; i < argc; i++) {
        if (is_argument) {
            is_argument = false;
            switch (last_option) {
            case LongOptionWithArgument::WINDOW_WIDTH:
                app.set_window_width(convert_string_to_int(argv[i]));
                break;
            case LongOptionWithArgument::WINDOW_HEIGHT:
                app.set_window_height(convert_string_to_int(argv[i]));
                break;
            case LongOptionWithArgument::GRID_WIDTH:
                app.set_grid_width(convert_string_to_int(argv[i]));
                break;
            case LongOptionWithArgument::GRID_HEIGHT:
                app.set_grid_height(convert_string_to_int(argv[i]));
                break;
            case LongOptionWithArgument::OBSTACLE_AMOUNT:
                app.set_obstacle_amount(convert_string_to_int(argv[i]));
                break;
            case LongOptionWithArgument::ALGORITHM:
                app.set_algorithm(argv[i]);
                break;
            }
        } else {
            if (std::strcmp(argv[i], "-auto") == 0) {
                app.set_mode(ApplicationMode::AUTO);
            } else if (std::strcmp(argv[i], "-step-through") == 0) {
                app.set_mode(ApplicationMode::STEP_THROUGH);
            } else if (std::strcmp(argv[i], "-help") == 0) {
                app.set_mode(ApplicationMode::HELP);
            } else if (std::strcmp(argv[i], "-list-defaults") == 0) {
                app.set_mode(ApplicationMode::DEFAULTS);
            } else if (std::strcmp(argv[i], "-list-algorithms") == 0) {
                app.set_mode(ApplicationMode::ALGORITHMS);
            } else if (std::strcmp(argv[i], "-window-width") == 0) {
                is_argument = true;
                last_option = LongOptionWithArgument::WINDOW_WIDTH;
            } else if (std::strcmp(argv[i], "-window-height") == 0) {
                is_argument = true;
                last_option = LongOptionWithArgument::WINDOW_HEIGHT;
            } else if (std::strcmp(argv[i], "-grid-width") == 0) {
                is_argument = true;
                last_option = LongOptionWithArgument::GRID_WIDTH;
            } else if (std::strcmp(argv[i], "-grid-height") == 0) {
                is_argument = true;
                last_option = LongOptionWithArgument::GRID_HEIGHT;
            } else if (std::strcmp(argv[i], "-obstacle-amount") == 0) {
                is_argument = true;
                last_option = LongOptionWithArgument::OBSTACLE_AMOUNT;
            } else if (std::strcmp(argv[i], "-algorithm") == 0) {
                is_argument = true;
                last_option = LongOptionWithArgument::ALGORITHM;
            }
        }
    }
}

// This function does as expected, but if it cannot convert string to int it
// exits the program.
int convert_string_to_int(char* string)
{
    char* endptr;
    int value;

    value = std::strtol(string, &endptr, 10);

    if (*endptr != '\0') {
        std::cout << "Could not convert string to integer." << std::endl;
        std::exit(-1);
    }
    return value;
}

//int main()
//{
//    Plotter plotter(sf::VideoMode(1000, 1000), 2, 4);
//    sf::Vector2i robot_position(0, 3);
//    std::vector<sf::Vector2i> obstacles = {
//        sf::Vector2i(0, 0),
//        sf::Vector2i(1, 1),
//        sf::Vector2i(1, 2),
//    };
//
//    while (plotter.is_open()) {
//        plotter.handle_events();
//        plotter.plot(robot_position, 3, obstacles);
//    }
//    return 0;
//}
