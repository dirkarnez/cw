cmake_minimum_required(VERSION 3.14)
project(app CXX)

include(FetchContent)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG yaml-cpp-0.6.3
)

set(YAML_CPP_BUILD_TESTS OFF)
set(YAML_CPP_BUILD_TOOLS OFF)
set(YAML_CPP_BUILD_CONTRIB OFF)
set(YAML_CPP_INSTALL OFF)

FetchContent_MakeAvailable(yaml-cpp)


FetchContent_Declare(
    elements
    GIT_REPOSITORY https://github.com/cycfi/elements.git
    GIT_TAG master
)

# set(ELEMENTS_BUILD_EXAMPLES OFF)

FetchContent_MakeAvailable(elements)

set(Boost_NO_SYSTEM_PATHS TRUE)
set(Boost_USE_STATIC_LIBS TRUE)
set(BOOST_ROOT "C:/Code Projects/Cpp Libraries/boost_1_74_0")

find_package(Boost REQUIRED COMPONENTS system filesystem)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(${PROJECT_NAME}
      "src/main.cpp"
      "src/foo_bar.h"
      "src/foo_bar.cpp")
  target_link_libraries(${PROJECT_NAME} 
      ${Boost_LIBRARIES}
      lua
      inputbox
      yaml-cpp
      elements)
endif()