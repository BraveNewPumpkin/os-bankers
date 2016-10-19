cmake_minimum_required(VERSION 2.8)
project(os_bankers)

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#find_package(Boost 1.45.0 COMPONENTS *boost libraries here*)
#
#if(Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIRS})
#    add_executable(progname file1.cxx file2.cxx)
#    target_link_libraries(progname ${Boost_LIBRARIES})
#endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp InputParser.cpp Process.cpp Bank.cpp Resource.cpp InterCom.cpp EdfSjfScheduler.cpp StrOp.cpp StrOp.h EdfScheduler.cpp EdfScheduler.h EdfLjfScheduler.cpp EdfLjfScheduler.h)
add_executable(os_bankers ${SOURCE_FILES})