cmake_minimum_required(VERSION 3.25)
project(flocktest C)

set(CMAKE_C_STANDARD 11)

find_package( Threads REQUIRED )
add_executable(flocktest main.c)
target_link_libraries( flocktest Threads::Threads )


