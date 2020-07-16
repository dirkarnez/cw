cmake_minimum_required(VERSION 3.0)
project(app)

set(Boost_NO_SYSTEM_PATHS TRUE)
set(Boost_USE_STATIC_LIBS TRUE)
set(BOOST_ROOT "D:/Cpp Libraries/boost_1_73_0")

find_package(Boost REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(${PROJECT_NAME}
      "src/main.cpp"
      "src/foo_bar.h"
      "src/foo_bar.cpp")
  target_link_libraries(${PROJECT_NAME} 
      ${Boost_LIBRARIES}
      inputbox)
endif()