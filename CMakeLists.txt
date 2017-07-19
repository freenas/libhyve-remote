project(libhyverem)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "-O2 -W -Wall -g")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(main libcheck.c vncserver.c main.c)
add_executable(test_full libcheck.c vncserver.c test.c)

set(libsrc libcheck.c vncserver.c)
add_library(hyverem SHARED ${libsrc})

install(TARGETS hyverem DESTINATION /usr/local/lib/)
