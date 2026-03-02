cmake_minimum_required(VERSION 3.20)
project(RCAEPrototype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(rcae_runtime_lib
    engine/runtime/mini/MiniEngine.cpp
)
target_include_directories(rcae_runtime_lib PUBLIC include)
set_target_properties(rcae_runtime_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(rcae_runtime_capi SHARED
    engine/runtime/mini/MiniEngineCAPI.cpp
)
target_link_libraries(rcae_runtime_capi PRIVATE rcae_runtime_lib)
target_include_directories(rcae_runtime_capi PUBLIC include)

add_library(rcae_rendering_lib
    engine/rendering/Renderer.cpp
)
target_include_directories(rcae_rendering_lib PUBLIC include)
set_target_properties(rcae_rendering_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(rcae_scene_lib
    engine/runtime/scene/SceneWorld.cpp
)
target_include_directories(rcae_scene_lib PUBLIC include)
set_target_properties(rcae_scene_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(rcae_scene_lib PUBLIC rcae_rendering_lib)

add_library(rcae_game_runtime_lib
    engine/runtime/GameRuntimeApp.cpp
    engine/runtime/FrameOrchestrator.cpp
)
target_include_directories(rcae_game_runtime_lib PUBLIC include)
set_target_properties(rcae_game_runtime_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(rcae_game_runtime_lib PUBLIC rcae_scene_lib)

add_library(rcae_game_runtime_capi SHARED
    engine/runtime/GameRuntimeCAPI.cpp
)
target_link_libraries(rcae_game_runtime_capi PRIVATE rcae_game_runtime_lib)
target_include_directories(rcae_game_runtime_capi PUBLIC include)

add_library(rcae_editor_lib
    tools/editor_cpp/src/EditorApp.cpp
)

target_include_directories(rcae_editor_lib PUBLIC tools/editor_cpp/include)

add_executable(rcae_editor tools/editor_cpp/src/main.cpp)
target_link_libraries(rcae_editor PRIVATE rcae_editor_lib)

add_executable(rcae_game_runtime_demo tools/game_runtime_demo.cpp)
target_link_libraries(rcae_game_runtime_demo PRIVATE rcae_runtime_lib)

add_executable(rcae_game_app tools/game_app/main.cpp)
target_link_libraries(rcae_game_app PRIVATE rcae_game_runtime_lib)

add_executable(rcae_rendering_demo tools/rendering/rendering_demo.cpp)
target_link_libraries(rcae_rendering_demo PRIVATE rcae_rendering_lib)

add_executable(rcae_scene_world_demo tools/scene/scene_world_demo.cpp)
target_link_libraries(rcae_scene_world_demo PRIVATE rcae_scene_lib)

enable_testing()

add_executable(rcae_editor_tests tests/cpp/test_editor.cpp)
target_link_libraries(rcae_editor_tests PRIVATE rcae_editor_lib)
add_test(NAME rcae_editor_tests COMMAND rcae_editor_tests)

add_executable(rcae_mini_engine_tests tests/cpp/test_mini_engine.cpp)
target_link_libraries(rcae_mini_engine_tests PRIVATE rcae_runtime_lib)
add_test(NAME rcae_mini_engine_tests COMMAND rcae_mini_engine_tests)

add_executable(rcae_mini_engine_capi_tests tests/cpp/test_mini_engine_capi.cpp)
target_link_libraries(rcae_mini_engine_capi_tests PRIVATE rcae_runtime_capi)
add_test(NAME rcae_mini_engine_capi_tests COMMAND rcae_mini_engine_capi_tests)
set_target_properties(rcae_mini_engine_capi_tests PROPERTIES BUILD_RPATH "$<TARGET_FILE_DIR:rcae_runtime_capi>")

add_executable(rcae_renderer_tests tests/cpp/test_renderer.cpp)
target_link_libraries(rcae_renderer_tests PRIVATE rcae_rendering_lib)
add_test(NAME rcae_renderer_tests COMMAND rcae_renderer_tests)

add_executable(rcae_scene_world_tests tests/cpp/test_scene_world.cpp)
target_link_libraries(rcae_scene_world_tests PRIVATE rcae_scene_lib)
add_test(NAME rcae_scene_world_tests COMMAND rcae_scene_world_tests)

add_executable(rcae_game_runtime_app_tests tests/cpp/test_game_runtime_app.cpp)
target_link_libraries(rcae_game_runtime_app_tests PRIVATE rcae_game_runtime_lib)
add_test(NAME rcae_game_runtime_app_tests COMMAND rcae_game_runtime_app_tests)

add_executable(rcae_game_runtime_capi_tests tests/cpp/test_game_runtime_capi.cpp)
target_link_libraries(rcae_game_runtime_capi_tests PRIVATE rcae_game_runtime_capi)
add_test(NAME rcae_game_runtime_capi_tests COMMAND rcae_game_runtime_capi_tests)
set_target_properties(rcae_game_runtime_capi_tests PROPERTIES BUILD_RPATH "$<TARGET_FILE_DIR:rcae_game_runtime_capi>")
