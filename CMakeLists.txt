cmake_minimum_required(VERSION 3.17)

# variables / options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#

project(GenericParser)
	
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/Constants
    ${CMAKE_SOURCE_DIR}/include/Concepts
    ${CMAKE_SOURCE_DIR}/include/Scanner
    ${CMAKE_SOURCE_DIR}/include/BaseParser
)
