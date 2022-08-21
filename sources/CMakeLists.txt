
set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp plotter.cpp robot_server.cpp
    application.cpp sfml_ui.cpp console_ui.cpp data_types.cpp
    algorithms/algorithms.cpp algorithms/random_algorithm.cpp
    algorithms/no_backtrack_random_algorithm.cpp algorithms/helper_functions.cpp
    algorithms/fast_deterministic_algorithm.cpp)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} sfml-main)
endif()
