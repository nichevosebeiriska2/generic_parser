cmake_minimum_required(VERSION 3.17)

# variables / options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
#

project(GenericParser)

#fetch 
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.9.0  # Рекомендуется фиксировать версию (тег или коммит)
)

FetchContent_MakeAvailable(googletest benchmark)



file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${CMAKE_SOURCE_DIR}/include/Scanner
    ${CMAKE_SOURCE_DIR}/include/base_parser
    ${CMAKE_SOURCE_DIR}/include/combined_parsers
    ${CMAKE_SOURCE_DIR}/include/examples
    ${CMAKE_SOURCE_DIR}/include/examples/Json
    ${CMAKE_SOURCE_DIR}/include/examples/calculator
    ${CMAKE_SOURCE_DIR}/include/Test
)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gmock GTest::gtest_main benchmark::benchmark)
