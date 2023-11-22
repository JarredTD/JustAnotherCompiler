cmake_minimum_required(VERSION 3.4)
project(JAC VERSION 1.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic")

add_executable(jac jac.c src/jaclx.c src/jacp.c src/jacsa.c src/jaccg.c)

