# RCAE Full Engine Delivery Plan (Section-by-Section)

This plan is designed so you can push to GitHub after each section and still converge toward a full engine by the end.

## How to use this plan

- Complete one section at a time.
- Open a PR per section.
- Do not start the next section until the previous section passes its acceptance checks.
- Tag each completed section (`v0.1-s1`, `v0.2-s2`, etc.) so rollback is easy.

---

## Section 1 — Runtime Core Foundation

### Goal
A stable native runtime loop and world state foundation.

### Scope
- Engine loop and fixed timestep update
- ECS-style entity/component storage
- Deterministic update ordering
- Basic serialization for scene state

### Definition of Done
- Runtime can create/load/update/save a scene.
- Basic simulation tests pass deterministically across runs.

### Acceptance checks
- `cmake -S . -B build`
- `cmake --build build`
- `ctest --test-dir build --output-on-failure`

### GitHub checkpoint
- PR title: `section-1: runtime core foundation`
- Tag: `v0.1-s1`

---

## Section 2 — C# Tooling Front-End + C++ Interop Contract

### Goal
Use C# for fast visual/editor iteration while keeping runtime performance in C++.

### Scope
- Stabilize C ABI surface (`MiniEngineCAPI`-style boundary)
- C# interop wrapper (`DllImport`) for core runtime calls
- Ownership rules (create/destroy/native handle safety)
- Error/result conventions across managed/native boundary

### Definition of Done
- C# layer can drive core simulation calls without crashes/leaks.
- Interop tests cover create/tick/render/dispose paths.

### Acceptance checks
- `cmake -S . -B build`
- `cmake --build build`
- `ctest --test-dir build --output-on-failure`

### GitHub checkpoint
- PR title: `section-2: managed frontend and native bridge contract`
- Tag: `v0.2-s2`

---

## Section 3 — Asset Pipeline and Packaging

### Goal
Make content ingestion and runtime packaging production-usable.

### Scope
- Import -> validate -> cook stages
- Asset dependency graph tracking
- Versioned package manifest format
- Runtime package mount and precedence checks

### Definition of Done
- Sample project assets cook and load end-to-end.
- Build catches invalid/missing dependencies.

### Acceptance checks
- `cmake -S . -B build`
- `cmake --build build`
- `ctest --test-dir build --output-on-failure`

### GitHub checkpoint
- PR title: `section-3: asset pipeline and package system`
- Tag: `v0.3-s3`

---

## Section 4 — Rendering Baseline

### Goal
Deliver a minimal but real rendering path suitable for game iteration.

### Scope
- Renderer bootstrap and frame graph baseline
- Material/shader binding flow
- Camera and scene traversal
- Basic lighting and draw submission profiling

### Definition of Done
- A sample scene renders reliably with measurable frame timing.
- Material updates reflect in runtime without full rebuild.

### Acceptance checks
- `cmake -S . -B build`
- `cmake --build build`
- `ctest --test-dir build --output-on-failure`
- Runtime smoke scene starts and renders correctly.

### GitHub checkpoint
- PR title: `section-4: rendering baseline`
- Tag: `v0.4-s4`

---

## Section 5 — Physics + Gameplay Movement

### Goal
Support responsive gameplay movement with stable world interactions.

### Scope
- Movement controller system
- World physics integration points
- Collision channels/layers
- Predictable handoff rules between movement and world simulation

### Definition of Done
- Character movement feels responsive and consistent.
- Physics regression tests protect collision behavior.

### Acceptance checks
- `cmake -S . -B build`
- `cmake --build build`
- `ctest --test-dir build --output-on-failure`

### GitHub checkpoint
- PR title: `section-5: gameplay movement and physics integration`
- Tag: `v0.5-s5`

---

## Section 6 — Editor Workflows (C#-First)

### Goal
Enable fast content creation loops for designers/artists.

### Scope
- Inspector + explorer + scene selection flow
- Script/property binding workflows
- Live preview and validation feedback
- Error surfaces for content authors

### Definition of Done
- Designer can author a small playable interaction loop entirely in editor workflows.
- Changes are visible with minimal turnaround.

### Acceptance checks
- Editor workflow test suite passes.
- Runtime integration smoke test passes.

### GitHub checkpoint
- PR title: `section-6: editor workflows and content authoring loop`
- Tag: `v0.6-s6`

---

## Section 7 — Networking/Save/Platform Services

### Goal
Add systems needed for production shipping reliability.

### Scope
- Save/load robustness and migration
- Networking layer hooks (if title needs multiplayer)
- Platform abstraction hardening
- Configuration and telemetry scaffolding

### Definition of Done
- Save compatibility checks pass across revisions.
- Platform/service layer boots cleanly in CI targets.

### Acceptance checks
- `cmake -S . -B build`
- `cmake --build build`
- `ctest --test-dir build --output-on-failure`

### GitHub checkpoint
- PR title: `section-7: platform and service hardening`
- Tag: `v0.7-s7`

---

## Section 8 — Production Hardening and Release Readiness

### Goal
Converge from prototype to releasable engine baseline.

### Scope
- Performance budgets and profiling gates
- Memory/resource leak checks
- Long-run soak tests
- CI quality gates and release checklist automation

### Definition of Done
- Engine meets agreed performance and stability gates.
- Release checklist can be executed from CI with reproducible outputs.

### Acceptance checks
- Full CI suite including stress/soak tests passes.
- Release candidate build artifacts generated and validated.

### GitHub checkpoint
- PR title: `section-8: release hardening and rc baseline`
- Tag: `v1.0-rc`

---

## Minimum weekly cadence recommendation

- Week N: deliver one section branch + PR + tag.
- Keep each section small enough to review in < 800 LOC where possible.
- If a section exceeds scope, split into `Nx.1`, `Nx.2` mini-sections.

## Rule for convergence to full engine

If every section's Definition of Done is met and tagged in order, you end with a full-engine baseline rather than an open-ended prototype.
