
// Includes
#include "application.h"
#include "sfml_ui.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

// Local types
enum class Mode {
    HELP,
    LIST_DEFAULTS,
    LIST_ALGORITHMS,
    RUN,
};

enum class LongOptionWithArgument {
    GRID_WIDTH,
    GRID_HEIGHT,
    OBSTACLE_AMOUNT,
    ALGORITHM,
};

// Global constants (defaults)
const Parameters DEFAULT_PARAMETERS = {4, 4, 4, "random"};
const Mode DEFAULT_MODE = Mode::RUN;

// Function prototypes
void parse_arguments(int argc, char* argv[], Parameters&, Mode&);
void print_help();
void print_parameters(const Parameters&);
int convert_string_to_int(char*);
int perform_mode(const Parameters&, Mode);
int run_program(const Parameters&);

int main(int argc, char* argv[])
{
    // Set defaults for parameters and mode
    Parameters parameters = DEFAULT_PARAMETERS;
    Mode mode = Mode::RUN;

    // Parse command line arguments and change parameters and mode
    parse_arguments(argc, argv, parameters, mode);

    // Let the user know how to access help
    if (mode != Mode::HELP) {
        std::cout << "Access help with -help" << std::endl;
        std::cout << std::endl;
    }

    // Perform mode
    return perform_mode(parameters, mode);
}

int perform_mode(const Parameters& parameters, Mode mode)
{
    int return_code = 0;

    switch (mode) {
    case Mode::HELP:
        print_help();
        break;
    case Mode::LIST_DEFAULTS:
        print_parameters(DEFAULT_PARAMETERS);
        break;
    case Mode::LIST_ALGORITHMS:
        {
            Application app(parameters);
            app.print_algorithms();
        }
        break;
    case Mode::RUN:
        print_parameters(parameters);
        return_code = run_program(parameters);
        break;
    }
    return return_code;
}

int run_program(const Parameters& parameters)
{
    SFMLUI ui(parameters);

    return ui.run_loop();
}

void parse_arguments(int argc, char* argv[], Parameters& parameters, Mode& mode)
{
    LongOptionWithArgument last_option;
    bool is_argument = false;

    for (int i = 1; i < argc; i++) {
        if (is_argument) {
            is_argument = false;
            switch (last_option) {
            case LongOptionWithArgument::GRID_WIDTH:
                parameters.grid_width = convert_string_to_int(argv[i]);
                break;
            case LongOptionWithArgument::GRID_HEIGHT:
                parameters.grid_height = convert_string_to_int(argv[i]);
                break;
            case LongOptionWithArgument::OBSTACLE_AMOUNT:
                parameters.obstacle_amount = convert_string_to_int(argv[i]);
                break;
            case LongOptionWithArgument::ALGORITHM:
                parameters.algorithm = argv[i];
                break;
            }
        } else {
            if (std::strcmp(argv[i], "-help") == 0) {
                mode = Mode::HELP;
            } else if (std::strcmp(argv[i], "-list-defaults") == 0) {
                mode = Mode::LIST_DEFAULTS;
            } else if (std::strcmp(argv[i], "-list-algorithms") == 0) {
                mode = Mode::LIST_ALGORITHMS;
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

void print_help()
{
    std::cout << "List of options:" << std::endl;
    std::cout << "  -help                   Print this help message" << std::endl;
    std::cout << "  -list-defaults          List the default settings for the simulation" << std::endl;
    std::cout << "  -list-algorithms        List the available mapping algorithms" << std::endl;
    std::cout << "  -grid-width [int]       Change the grid width" << std::endl;
    std::cout << "  -grid-height [int]      Change the grid height" << std::endl;
    std::cout << "  -obstacle-amount [int]  Change the amount of obstacles" << std::endl;
    std::cout << "  -algorithm [string]     Change the algorithm used" << std::endl;
}

void print_parameters(const Parameters& parameters)
{
    std::cout << "grid width:      " << parameters.grid_width << std::endl;
    std::cout << "grid height:     " << parameters.grid_height << std::endl;
    std::cout << "obstacle amount: " << parameters.obstacle_amount << std::endl;
    std::cout << "algorithm:       " << parameters.algorithm << std::endl;
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
