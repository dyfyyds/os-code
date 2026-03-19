cmake_minimum_required(VERSION 3.10)
project(OSProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




add_executable(OSProject main.cpp Process.cpp Scheduler.cpp)
