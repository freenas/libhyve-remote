cmake_minimum_required(VERSION 2.8)
add_executable(main libcheck.c vncserver.c main.c)

add_executable(test_it test.c)

set(libsrc libcheck.c vncserver.c)
add_library(hyverem SHARED ${libsrc})
