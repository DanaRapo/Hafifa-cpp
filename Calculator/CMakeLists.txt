cmake_minimum_required(VERSION 3.10)

project(Calculator)

include_directories(Include)

set(SOURCES
src/main.cpp
src/BasicCalculator.cpp
src/AdvanceCalculator.cpp
)

add_executable(Calculator ${SOURCES})