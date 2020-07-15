cmake_minimum_required(VERSION 3.0)
project(app)

add_executable(${PROJECT_NAME}
    "src/main.cpp"
    "src/foo_bar.h"
    "src/foo_bar.cpp")
target_link_libraries(${PROJECT_NAME}
    inputbox)
