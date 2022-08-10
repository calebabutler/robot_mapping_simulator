
set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp plotter.cpp)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-main)
else()
    target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
endif()
