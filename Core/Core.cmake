cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Core)

get_filename_component(CoreDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(CoreSources
    ${CoreDir}/Assert.hpp
    ${CoreDir}/Utils.hpp
    ${CoreDir}/VectorDetails.hpp
    ${CoreDir}/VectorDetails.ipp
    ${CoreDir}/Vector.hpp
    ${CoreDir}/Vector.ipp
    ${CoreDir}/FlatVector.hpp
    ${CoreDir}/FlatString.hpp
    ${CoreDir}/Core.cpp
)

add_library(${PROJECT_NAME} ${CoreSources})

target_include_directories(${PROJECT_NAME} PUBLIC ${CoreDir}/..)

target_link_libraries(${PROJECT_NAME} PUBLIC coverage_config)
