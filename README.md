# RCAE Project

This is the complete project structure for the RCAE my custom game engine that is being developed in C++ and C# with LUA on the side for other features.

## Core Engine Files

- `engine.h`: Contains the main engine class and logic.
- `Replace Me`: replace this later when i got stuff.

## Test Files

- `Replace Me`: replace this later when i got stuff.

## Other Files

- `Replace Me`: replace this later when i got stuff.

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
