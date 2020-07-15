cmake_minimum_required(VERSION 3.0)
project(inputbox)

add_library(${PROJECT_NAME} STATIC
    "include/inputbox.h"
    "src/inputbox.cpp")
target_include_directories(${PROJECT_NAME}
    PUBLIC
    "include")
