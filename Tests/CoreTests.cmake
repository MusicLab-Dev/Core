project(CoreTests)

find_package(GTest REQUIRED)

get_filename_component(CoreTestsDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(CoreTestsSources
    ${CoreTestsDir}/tests_Vector.cpp
    ${CoreTestsDir}/tests_FlatString.cpp
    ${CoreTestsDir}/tests_UniqueAlloc.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${CoreTestsSources})

add_test(NAME ${CMAKE_PROJECT_NAME} COMMAND ${CMAKE_PROJECT_NAME})

target_link_libraries(${CMAKE_PROJECT_NAME}
PUBLIC
    Core
    GTest::GTest GTest::Main
)