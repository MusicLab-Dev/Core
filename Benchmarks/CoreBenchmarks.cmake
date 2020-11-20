project(CoreBenchmarks)

find_package(benchmark REQUIRED)

get_filename_component(CoreBenchmarksDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(CoreBenchmarksSources
    ${CoreBenchmarksDir}/Main.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${CoreBenchmarksSources})

target_link_libraries(${CMAKE_PROJECT_NAME}
PUBLIC
    Core
    benchmark::benchmark
)
