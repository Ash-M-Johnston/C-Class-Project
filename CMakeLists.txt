cmake_minimum_required(VERSION 3.30)
project(Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Project main.c
        server.c
        server.h
        file.c
        file.h
        env.c
        env.h
)
