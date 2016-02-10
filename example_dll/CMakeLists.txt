cmake_minimum_required (VERSION 3.0)

set(INTERCEPT_EXAMPLE_NAME "example_dll")

add_definitions(/DINTERCEPT_NO_THREAD_SAFETY)
set(INTERCEPT_NAME "example_dll")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}example_dll")

file(GLOB INTERCEPT_EXAMPLE_SOURCES *.h *.hpp *.c *.cpp)

SOURCE_GROUP("example_dll" FILES ${INTERCEPT_EXAMPLE_SOURCES})
#SOURCE_GROUP("intercept_headers" FILES ${INTERCEPT_CLIENT_SOURCES})
#SOURCE_GROUP("intercept_headers\\client" FILES ${INTERCEPT_CLIENT_MAIN_SOURCES})
#SOURCE_GROUP("intercept_headers\\shared" FILES ${INTERCEPT_CLIENT_SHARED_SOURCES})
#SOURCE_GROUP("intercept_headers\\client\\sqf" FILES ${INTERCEPT_CLIENT_SQF_SHARED_SOURCES})


add_library( ${INTERCEPT_EXAMPLE_NAME} SHARED ${INTERCEPT_EXAMPLE_SOURCES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} "intercept/include/")
target_link_libraries(${INTERCEPT_EXAMPLE_NAME} ${CMAKE_SOURCE_DIR}/intercept/lib/intercept_client.lib)

target_link_libraries(${INTERCEPT_EXAMPLE_NAME} ${LINK_LIBS})
set_target_properties(${INTERCEPT_EXAMPLE_NAME} PROPERTIES PREFIX "")
set_target_properties(${INTERCEPT_EXAMPLE_NAME} PROPERTIES FOLDER example_dll)

if(CMAKE_COMPILER_IS_GNUCXX)
	set_target_properties(${INTERCEPT_EXAMPLE_NAME} PROPERTIES LINK_SEARCH_START_STATIC 1)
	set_target_properties(${INTERCEPT_EXAMPLE_NAME} PROPERTIES LINK_SEARCH_END_STATIC 1)
endif()