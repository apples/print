cmake_minimum_required (VERSION 3.2)
project(print)

file(GLOB_RECURSE print_SOURCES *.hpp)
add_library(print INTERFACE)
set_property(TARGET print PROPERTY INTERFACE_SOURCES ${print_SOURCES})
target_include_directories(print INTERFACE .)
