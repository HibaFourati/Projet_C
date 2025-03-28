cmake_minimum_required(VERSION 3.29)
project(ESSAI C)

set(CMAKE_C_STANDARD 11)

add_executable(ESSAI main.c
        MAP.h
        PLAYER.h
        MAP.c
        PLAYER.c)
