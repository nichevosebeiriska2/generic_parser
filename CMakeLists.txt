cmake_minimum_required(VERSION 3.10)
project(GenericParser VERSION 1.0.0 LANGUAGES CXX)

# т.к. решено было делать либу на концепта (а иначе ты ее не задебажишь просто напросто, callstack при ошибках на 50+ вызовов проваливается, глазам больно), берем 20-й стандарт
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================================
# СОЗДАНИЕ HEADER-ONLY ЛИБЫ
# ==============================================================================


# делаем либу интерфейсом, т.к. она header-only. в любой проект куда будет подключена просто 
# пробросит свои пути до include. Нам вцелом только common/parsing.h и нужен. Там вся логика имеется.
# TODO : надо разобраться как закрыть доступ к другим частям либы, чтобы юзер не видел в неймспейсе остальной хлам, который без понимания вообще никак не использовать
add_library(GenericParser INTERFACE)
add_library(GenericParser::GenericParser ALIAS GenericParser)

# Указываем пути к заголовкам.
# BUILD_INTERFACE: работает, когда библиотека собирается как часть проекта (или через FetchContent)
# INSTALL_INTERFACE: будет работать, если библиотеку установят в систему через 'make install'
target_include_directories(GenericParser INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# либа на концептах, так что требует с++20
target_compile_features(GenericParser INTERFACE cxx_std_20)

# ==============================================================================
# ОПЦИИ СБОРКИ (пока что только тесты)
# ==============================================================================

# По умолчанию ВЫКЛЮЧЕНЫ, чтобы не замедлять сборку при использовании как зависимости
option(BUILD_TESTS "Build unit tests (requires GTest)" ON) # по дефолту билдим тесты

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
