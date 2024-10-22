cmake_minimum_required(VERSION 3.10)

project(TapeSort VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0167 NEW)


find_package(Boost 1.75 REQUIRED COMPONENTS json)


include_directories(${Boost_INCLUDE_DIRS})


add_executable(TapeSort main.cpp TapeAlgorithms.cpp)


target_link_libraries(TapeSort PUBLIC Boost::json)
include_directories(./) 
