cmake_minimum_required (VERSION 3.0)

if(NOT INTERCEPT_INCLUDE_DIR)
    set(INTERCEPT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/intercept/include")
endif()
if(INTERCEPT_INCLUDE_DIR)
    set(INTERCEPT_LIB_INCLUDED "true")
endif()

add_subdirectory(example_dll)