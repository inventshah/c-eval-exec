# Project's name
PROJECT ( ceval C )

# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.1)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(${PROJECT_NAME} STATIC 
	${PROJECT_SOURCE_DIR}/src/eval.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "include/eval.h")

add_executable(test ${PROJECT_SOURCE_DIR}/tests.c)
target_link_libraries(test ${PROJECT_NAME})

INSTALL(TARGETS ${PROJECT_NAME} 
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
)