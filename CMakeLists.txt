cmake_minimum_required(VERSION 3.13)
project(IRC C)

set(CMAKE_C_STANDARD 99)

add_executable(IRC srcs/cores/main.c incs/irc.h srcs/server/server.c)