cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Core)

get_filename_component(CoreDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(CoreSources
    ${CoreDir}/Assert.hpp
    ${CoreDir}/Utils.hpp
    ${CoreDir}/Utils.ipp
    ${CoreDir}/HeapArray.hpp
    ${CoreDir}/HeapArray.ipp
    ${CoreDir}/VectorDetails.hpp
    ${CoreDir}/VectorDetails.ipp
    ${CoreDir}/SortedVectorDetails.hpp
    ${CoreDir}/SortedVectorDetails.ipp
    ${CoreDir}/VectorBase.hpp
    ${CoreDir}/VectorBase.ipp
    ${CoreDir}/Vector.hpp
    ${CoreDir}/SortedVector.hpp
    ${CoreDir}/FlatVectorBase.hpp
    ${CoreDir}/FlatVectorBase.ipp
    ${CoreDir}/FlatVector.hpp
    ${CoreDir}/SortedFlatVector.hpp
    ${CoreDir}/SmallVectorBase.hpp
    ${CoreDir}/SmallVectorBase.ipp
    ${CoreDir}/SmallVector.hpp
    ${CoreDir}/SortedSmallVector.hpp
    ${CoreDir}/FlatString.hpp
    ${CoreDir}/Core.cpp
    ${CoreDir}/Functor.hpp
    ${CoreDir}/TrivialFunctor.hpp
    ${CoreDir}/DispatcherDetails.hpp
    ${CoreDir}/Dispatcher.hpp
    ${CoreDir}/TrivialDispatcher.hpp
    ${CoreDir}/SPSCQueue.hpp
    ${CoreDir}/SPSCQueue.ipp
    ${CoreDir}/MPMCQueue.hpp
    ${CoreDir}/MPMCQueue.ipp
)

add_library(${PROJECT_NAME} ${CoreSources})

target_include_directories(${PROJECT_NAME} PUBLIC ${CoreDir}/..)

if(CODE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PUBLIC --coverage)
    target_link_options(${PROJECT_NAME} PUBLIC --coverage)
endif()