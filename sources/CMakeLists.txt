
set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp plotter.cpp robot_server.cpp
    application.cpp algorithms.cpp random_algorithm.cpp
    no_backtrack_random_algorithm.cpp)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} sfml-main)
endif()
