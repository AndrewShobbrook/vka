cmake_minimum_required(VERSION 3.13)

project(vka)

add_library(VMAUsage VMAUsage.cpp)

target_sources(vka PRIVATE utils.c)

add_subdirectory(vulkan_abs)
