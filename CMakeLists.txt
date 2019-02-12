cmake_minimum_required(VERSION 3.11)

project(catnip C)
find_package(Curses REQUIRED)
add_executable(catnip src/catnip.c)
target_include_directories(catnip PRIVATE include)
target_link_libraries(catnip ${CURSES_LIBRARIES} m)
