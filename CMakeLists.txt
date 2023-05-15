cmake_minimum_required(VERSION 3.24)
project(so_task_6 C)

set(CMAKE_C_STANDARD 23)

add_executable(so_task_6 so_task_6.c simple_semaphore.h simple_semaphore.c duplicator.c)
