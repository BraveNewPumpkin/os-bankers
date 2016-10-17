cmake_minimum_required(VERSION 2.8)
project(os_bankers)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(BOOST_DATE_TIME_NO_LIB 1)
set(BOOST_INCLUDEDIR ./include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
find_package(Boost 1.62.0 COMPONENTS)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(SOURCE_FILES main.cpp InputParser.cpp Process.cpp Bank.cpp Resource.cpp InterCom.cpp EdfSjfScheduler.cpp SyncedSharedUnsignedInt.cpp)
    add_executable(os_bankers ${SOURCE_FILES})
    target_link_libraries(os_bankers ${Boost_LIBRARIES})
endif()


