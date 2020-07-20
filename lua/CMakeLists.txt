project(lua)

file(GLOB LUA_HEADERS include/lua/*.h include/lua/*.hpp)
file(GLOB LUA_SOURCES src/*.c)

add_library(lua STATIC ${LUA_SOURCES} ${LUA_HEADERS})

target_include_directories(lua PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/lua")