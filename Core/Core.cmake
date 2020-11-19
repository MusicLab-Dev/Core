cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

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
    ${CoreDir}/FlatString.ipp
    ${CoreDir}/Core.cpp
)

add_library(${CMAKE_PROJECT_NAME} ${CoreSources})

message("CorePath: " ${CoreDir}/..)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "D:\\Projects\\MusicLab\\Core")
