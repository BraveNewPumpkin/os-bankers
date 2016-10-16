cmake_minimum_required(VERSION 2.8)
project(test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp InputParser.cpp Process.cpp Bank.cpp Resource.cpp InterCom.cpp EdfSjfScheduler.cpp EdfSjfScheduler.h)
add_executable(os_bankers ${SOURCE_FILES})