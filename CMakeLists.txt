cmake_minimum_required(VERSION 3.15)
project(robot_mapping_simulator)

if(WIN32)
    add_subdirectory(SFML/SFML-2.5.1)
endif()

add_subdirectory(sources)
