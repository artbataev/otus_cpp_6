cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    project(test_matrix VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(test_matrix VERSION 0.0.1)
endif()

set(CMAKE_CXX_STANDARD 17)
find_package(GTest REQUIRED)

file(GLOB TEST_SRC_FILES *.cpp)
include_directories("/usr/local/include")

foreach(_test_file ${TEST_SRC_FILES})
    get_filename_component(_test_name ${_test_file} NAME_WE)
    add_executable(${_test_name} ${_test_file})
    target_link_libraries(${_test_name} ${GTEST_LIBRARIES})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_link_libraries(${_test_name} pthread)
    endif()
    add_test(${_test_name} ${_test_name})
    set_tests_properties(${_test_name} PROPERTIES TIMEOUT 5)
endforeach()
