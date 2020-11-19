cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Core)

get_filename_component(CoreRoot ${CMAKE_CURRENT_LIST_FILE} PATH)

include(${CoreRoot}/Core/Core.cmake)

if(BUILD_TESTS)
    include(${CoreRoot}/Tests/CoreTests.cmake)
endif()

if(BUILD_BENCHMARKS)
    include(${CoreRoot}/Benchmarks/CoreBenchmarks.cmake)
endif()