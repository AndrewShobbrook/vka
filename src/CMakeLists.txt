cmake_minimum_required(VERSION 3.13)

project(vka)

add_library(VMAUsage VMAUsage.cpp)

add_subdirectory(vulkan_abs)
