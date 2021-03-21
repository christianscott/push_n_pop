cmake_minimum_required(VERSION 3.17)
project(push_n_pop C)

set(CMAKE_C_STANDARD 11)

add_executable(push_n_pop main.c lex.c lex.h vm.c vm.h vendor/nothings/stb/stretchy_buffer.h)