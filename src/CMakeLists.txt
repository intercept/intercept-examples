cmake_minimum_required (VERSION 3.0)

if(NOT INTERCEPT_EXAMPLE_SRC)
    if(NOT INTERCEPT_INCLUDE_PATH)
        set(INTERCEPT_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/intercept/include")
    endif()
    set(INTERCEPT_OUTPUT_DIR ${PROJECT_SOURCE_DIR})
endif()
if(INTERCEPT_EXAMPLE_SRC)
    set(INTERCEPT_LIB_INCLUDED "true")
    set(INTERCEPT_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/client/headers")
    set(INTERCEPT_OUTPUT_DIR ${INTERCEPT_EXAMPLE_SRC})
endif()

if(NOT INTERCEPT_LIB_PATH)
    set(INTERCEPT_LIB_PATH "${CMAKE_SOURCE_DIR}/intercept/lib/intercept_client.lib")
endif()

add_subdirectory(example_dll)
add_subdirectory(aps_demo)
add_subdirectory(laser_demo)