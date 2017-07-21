project(libhyverem_examples)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "-O2 -W -Wall -g -I../includes/")
set(CMAKE_BUILD_TYPE Debug)

add_executable(simple_output ../libcheck.c ../vncserver.c simple_output.c)
add_executable(simple_output_http ../libcheck.c ../vncserver.c simple_output_http.c)
