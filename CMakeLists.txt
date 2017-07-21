project(libhyverem)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "-O2 -W -Wall -I./includes -g")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

set(libsrc libcheck.c vncserver.c)
add_library(hyverem SHARED ${libsrc})

install(TARGETS hyverem DESTINATION /usr/local/lib/)
