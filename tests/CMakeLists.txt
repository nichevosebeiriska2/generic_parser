include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_SHALLOW    TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Собираем ВСЕ .cpp файлы (и тесты, и примеры)
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(GenericParserTests ${TEST_SOURCES})

# Линкуем библиотеку и gtest_main (он дает нам main())
target_link_libraries(GenericParserTests PRIVATE 
    GenericParser::GenericParser 
    GTest::gtest_main  # <-- ВАЖНО: это дает main() автоматически
)

include(GoogleTest)
gtest_discover_tests(GenericParserTests)