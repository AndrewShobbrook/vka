cmake_minimum_required(VERSION 3.13)

project(vka)

target_sources(vka PRIVATE devices.c commands.c pipelines.c swapchain.c render_passes.c)
