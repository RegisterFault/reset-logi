cmake_minimum_required(VERSION 3.10)

project(reset-logi)
add_executable(resetlogi main.c)
target_link_libraries(resetlogi usb-1.0)
