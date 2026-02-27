cmake_minimum_required(VERSION 3.20)
project(RCAEPrototype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(rcae_editor_lib
    tools/editor_cpp/src/EditorApp.cpp
)

target_include_directories(rcae_editor_lib PUBLIC tools/editor_cpp/include)

add_executable(rcae_editor tools/editor_cpp/src/main.cpp)
target_link_libraries(rcae_editor PRIVATE rcae_editor_lib)

enable_testing()
add_executable(rcae_editor_tests tests/cpp/test_editor.cpp)
target_link_libraries(rcae_editor_tests PRIVATE rcae_editor_lib)
add_test(NAME rcae_editor_tests COMMAND rcae_editor_tests)
