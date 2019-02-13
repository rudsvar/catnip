cmake_minimum_required(VERSION 3.11)

project(catnip C)
find_package(Curses REQUIRED)

add_executable(catnip src/catnip.c src/vertex.c)
target_include_directories(catnip PRIVATE include)
target_link_libraries(catnip m ${CURSES_LIBRARIES})

enable_testing()
add_executable(vertex_test test/vertex_test.c src/vertex.c)
target_include_directories(vertex_test PRIVATE include)
target_link_libraries(vertex_test m ${CURSES_LIBRARIES})
add_test(vertex_test vertex_test)
