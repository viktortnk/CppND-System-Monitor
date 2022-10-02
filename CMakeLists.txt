cmake_minimum_required(VERSION 2.9)
project(monitor)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "-std=c++17")

find_package(Curses REQUIRED)
# include_directories(${CURSES_INCLUDE_DIRS})

# include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})
target_include_directories(monitor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(monitor ${CURSES_LIBRARIES})
target_include_directories(monitor PUBLIC ${CURSES_INCLUDE_DIRS})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
