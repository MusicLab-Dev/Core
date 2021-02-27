cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Core)

get_filename_component(CoreDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(CoreSources
    ${CoreDir}/AllocatedFlatString.hpp
    ${CoreDir}/AllocatedFlatVector.hpp
    ${CoreDir}/AllocatedFlatVectorBase.hpp
    ${CoreDir}/AllocatedSmallString.hpp
    ${CoreDir}/AllocatedSmallVector.hpp
    ${CoreDir}/AllocatedSmallVectorBase.hpp
    ${CoreDir}/AllocatedString.hpp
    ${CoreDir}/AllocatedVector.hpp
    ${CoreDir}/AllocatedVectorBase.hpp
    ${CoreDir}/Assert.hpp
    ${CoreDir}/Core.cmake
    ${CoreDir}/Core.cpp
    ${CoreDir}/Dispatcher.hpp
    ${CoreDir}/DispatcherDetails.hpp
    ${CoreDir}/FlatString.hpp
    ${CoreDir}/FlatVector.hpp
    ${CoreDir}/FlatVectorBase.hpp
    ${CoreDir}/FlatVectorBase.ipp
    ${CoreDir}/Functor.hpp
    ${CoreDir}/Hash.hpp
    ${CoreDir}/HeapArray.hpp
    ${CoreDir}/HeapArray.ipp
    ${CoreDir}/MPMCQueue.hpp
    ${CoreDir}/MPMCQueue.ipp
    ${CoreDir}/SmallString.hpp
    ${CoreDir}/SmallVector.hpp
    ${CoreDir}/SmallVectorBase.hpp
    ${CoreDir}/SmallVectorBase.ipp
    ${CoreDir}/SortedAllocatedFlatVector.hpp
    ${CoreDir}/SortedAllocatedSmallVector.hpp
    ${CoreDir}/SortedAllocatedVector.hpp
    ${CoreDir}/SortedFlatVector.hpp
    ${CoreDir}/SortedSmallVector.hpp
    ${CoreDir}/SortedVector.hpp
    ${CoreDir}/SortedVectorDetails.hpp
    ${CoreDir}/SortedVectorDetails.ipp
    ${CoreDir}/SPSCQueue.hpp
    ${CoreDir}/SPSCQueue.ipp
    ${CoreDir}/String.hpp
    ${CoreDir}/StringDetails.hpp
    ${CoreDir}/StringLiteral.hpp
    ${CoreDir}/TrivialDispatcher.hpp
    ${CoreDir}/TrivialFunctor.hpp
    ${CoreDir}/UniqueAlloc.hpp
    ${CoreDir}/Utils.hpp
    ${CoreDir}/Utils.ipp
    ${CoreDir}/Vector.hpp
    ${CoreDir}/VectorBase.hpp
    ${CoreDir}/VectorBase.ipp
    ${CoreDir}/VectorDetails.hpp
    ${CoreDir}/VectorDetails.ipp
)

add_library(${PROJECT_NAME} ${CoreSources})

target_include_directories(${PROJECT_NAME} PUBLIC ${CoreDir}/..)

if(CODE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PUBLIC --coverage)
    target_link_options(${PROJECT_NAME} PUBLIC --coverage)
endif()