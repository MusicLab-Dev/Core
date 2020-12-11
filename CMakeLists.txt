cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

get_filename_component(CoreRoot ${CMAKE_CURRENT_LIST_FILE} PATH)
include(${CoreRoot}/CompileOptions/CompileOptions.cmake)

project(Core)



include(${CoreRoot}/Core/Core.cmake)

if(TESTS)
    enable_testing()
    include(${CoreRoot}/Tests/CoreTests.cmake)
endif()

if(BENCHMARKS)
    include(${CoreRoot}/Benchmarks/CoreBenchmarks.cmake)
endif()