find_package(Curses REQUIRED)

add_library(Vertex STATIC vertex.c)
target_link_libraries(Vertex m ${CURSES_LIBRARIES})
target_include_directories(Vertex PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(catnip catnip.c)
target_link_libraries(catnip Vertex)
