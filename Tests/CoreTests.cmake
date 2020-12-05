project(CoreTests)

find_package(GTest REQUIRED)

get_filename_component(CoreTestsDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(CoreTestsSources
    ${CoreTestsDir}/tests_Vector.cpp
    ${CoreTestsDir}/tests_FlatString.cpp
    ${CoreTestsDir}/tests_HeapArray.cpp
    ${CoreTestsDir}/tests_UniqueAlloc.cpp
    ${CoreTestsDir}/tests_Functor.cpp
    ${CoreTestsDir}/tests_TrivialFunctor.cpp
    ${CoreTestsDir}/tests_Dispatcher.cpp
)

add_executable(${PROJECT_NAME} ${CoreTestsSources})

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
PUBLIC
    Core
    GTest::GTest GTest::Main
)