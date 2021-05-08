cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Core)

get_filename_component(CoreDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(CorePrecompiledHeaders
    ${CoreDir}/AllocatedFlatString.hpp
    ${CoreDir}/AllocatedFlatVector.hpp
    ${CoreDir}/AllocatedSmallString.hpp
    ${CoreDir}/AllocatedSmallVector.hpp
    ${CoreDir}/AllocatedString.hpp
    ${CoreDir}/AllocatedVector.hpp
    ${CoreDir}/Assert.hpp
    ${CoreDir}/Dispatcher.hpp
    ${CoreDir}/DispatcherDetails.hpp
    ${CoreDir}/FlatString.hpp
    ${CoreDir}/FlatVector.hpp
    ${CoreDir}/Functor.hpp
    ${CoreDir}/Hash.hpp
    ${CoreDir}/HeapArray.hpp
    ${CoreDir}/MPMCQueue.hpp
    ${CoreDir}/SmallString.hpp
    ${CoreDir}/SmallVector.hpp
    ${CoreDir}/SortedAllocatedFlatVector.hpp
    ${CoreDir}/SortedAllocatedSmallVector.hpp
    ${CoreDir}/SortedAllocatedVector.hpp
    ${CoreDir}/SortedFlatVector.hpp
    ${CoreDir}/SortedSmallVector.hpp
    ${CoreDir}/SortedVector.hpp
    ${CoreDir}/SortedVectorDetails.hpp
    ${CoreDir}/SPSCQueue.hpp
    ${CoreDir}/String.hpp
    ${CoreDir}/StringDetails.hpp
    ${CoreDir}/StringLiteral.hpp
    ${CoreDir}/TrivialDispatcher.hpp
    ${CoreDir}/TrivialFunctor.hpp
    ${CoreDir}/UniqueAlloc.hpp
    ${CoreDir}/MacroUtils.hpp
    ${CoreDir}/Utils.hpp
    ${CoreDir}/Vector.hpp
    ${CoreDir}/VectorDetails.hpp
)

set(CoreSources
    ${CorePrecompiledHeaders}
    ${CoreDir}/AllocatedFlatVectorBase.hpp
    ${CoreDir}/AllocatedSmallVectorBase.hpp
    ${CoreDir}/AllocatedVectorBase.hpp
    ${CoreDir}/FlatVectorBase.hpp
    ${CoreDir}/SmallVectorBase.hpp
    ${CoreDir}/VectorBase.hpp
    ${CoreDir}/Core.cpp
    ${CoreDir}/FlatVectorBase.ipp
    ${CoreDir}/HeapArray.ipp
    ${CoreDir}/MPMCQueue.ipp
    ${CoreDir}/SmallVectorBase.ipp
    ${CoreDir}/SortedVectorDetails.ipp
    ${CoreDir}/SPSCQueue.ipp
    ${CoreDir}/Utils.ipp
    ${CoreDir}/VectorBase.ipp
    ${CoreDir}/VectorDetails.ipp
)

add_library(${PROJECT_NAME} ${CoreSources})

target_precompile_headers(${PROJECT_NAME} PUBLIC ${CorePrecompiledHeaders})

target_include_directories(${PROJECT_NAME} PUBLIC ${CoreDir}/..)

if(CODE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PUBLIC --coverage)
    target_link_options(${PROJECT_NAME} PUBLIC --coverage)
endif()
