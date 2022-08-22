cmake_minimum_required(VERSION 3.15)
project(robot_mapping_simulator)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    add_subdirectory(SFML/SFML-2.5.1)
endif()

add_executable(${PROJECT_NAME} sources/main.cpp sources/plotter.cpp
    sources/robot_server.cpp sources/application.cpp sources/sfml_ui.cpp
    sources/console_ui.cpp sources/data_types.cpp
    sources/algorithms/algorithms.cpp sources/algorithms/random_algorithm.cpp
    sources/algorithms/no_backtrack_random_algorithm.cpp
    sources/algorithms/helper_functions.cpp
    sources/algorithms/fast_deterministic_algorithm.cpp)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} sfml-main)
endif()
