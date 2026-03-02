# RCAE Project

This is the complete project structure for the RCAE my custom game engine that is being developed in C++ and C# with LUA on the side for other features.

## Core Engine Files

- `engine.py`: Contains the main engine class and logic.
- `utils.py`: Utility functions used by the engine.
- `config.py`: Configuration settings for the engine.

## Test Files

- `test_engine.py`: Unit tests for the engine functionality.
- `test_utils.py`: Unit tests for utility functions.

## Other Files

- `README.md`: Overview and documentation for the project.
- `requirements.txt`: Python package dependencies.

## Delivery Plan

For a section-by-section path to a full engine (with push checkpoints and acceptance gates), see:

- `docs/ENGINE_DELIVERY_SECTIONS.md`


## Playable Runtime Demo

Build and run the interactive runtime slice:

- `cmake -S . -B build`
- `cmake --build build`
- `./build/rcae_game_app games/game_alpha/content/levels/level_01.map`


Inside `rcae_game_app`, supported runtime commands include `w/a/s/d`, `tick`, `run <n>`, `click <button_id>`, `show`, and `quit`.

- C# visual bridge sample: `tools/editor/workflows/selection_binding/GameRuntimeVisualBridge.cs` (drives `rcae_game_runtime_capi`).
