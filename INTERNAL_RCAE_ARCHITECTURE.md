# RCAE Internal Architecture Notes (Private)

> Status: Internal planning document. Not for public release.

This document captures the direction for RCAE as a modular, Source-inspired engine with shared systems, cross-project assets, and a creator-focused editor workflow.

## 1) Technology Stack and Language Strategy

### Recommended language split
To support performance-heavy runtime systems and fast tool iteration:

- **C++** for engine/runtime core:
  - rendering backend
  - memory + resource management
  - physics integration
  - animation runtime
  - low-level scene systems
- **C#** for editor tooling and gameplay scripting APIs:
  - editor UI and tool extensions
  - gameplay scripts and component behaviors
  - rapid iteration logic that does not require native recompiles
- **Shader languages**:
  - HLSL/GLSL (or a unified cross-compile workflow) for material/shader authoring
- **Data formats**:
  - JSON/YAML/TOML for human-editable configs
  - binary cooked assets for runtime shipping builds

### Why this split
- C++ preserves Source-like control and performance for runtime-critical systems.
- C# enables faster scripting/tool UX iteration for designers.
- Clear ABI/API boundaries prevent gameplay scripts from tightly coupling to engine internals.

### Scripting model
- Expose a stable **component/property API** to scripts.
- Support hot reload for script assemblies where safe.
- Keep gameplay scripts sandboxed to avoid direct unsafe memory access.

## 2) Highly Modular Engine Architecture

### Core principle
RCAE is organized as independently upgradable subsystems so engine-level improvements can ship without rewriting the full stack.

### Target module boundaries
- Rendering (materials, lighting, post-processing)
- Physics (world simulation + movement controller layer)
- Audio (mixing, spatialization, DSP)
- Asset pipeline (import, compile, runtime package loading)
- AI/gameplay runtime (navigation, behavior scheduling, event graph)
- Networking/runtime services (if enabled per game)
- Tools/runtime integration layer

### Branching strategy
Use branch families for long-running products:
- `rcae/base` for common engine maintenance
- feature/product branches (example: `rcae/co-op`, `rcae/sp-horror`) for specialized systems

Each branch can carry product-specific additions while inheriting fixes and platform updates from `rcae/base` on a regular merge cadence.

### Retroactive update policy
For shared titles on the same compatibility tier:
- Security fixes and stability improvements flow back by default.
- Rendering/physics upgrades can be opt-in behind compatibility flags.
- Major behavior changes require migration notes and verification per title.

## 3) Editor UX: Inspector, Scripts, Explorer, Tools

### Core editor layout
The editor should include the following default panes:
- **Viewport**: world interaction, gizmos, scene preview
- **Explorer**: hierarchy/project tree for scenes, entities, assets, and packages
- **Properties Manager (Inspector)**: context-sensitive properties for selected object
- **Console/Output**: compile errors, import logs, runtime warnings
- **Tool Shelf**: quick-access utilities (terrain, materials, lighting, nav, packaging)

### Selection → Properties workflow (required)
When a user clicks an entity/object:
1. Selection is highlighted in viewport and explorer.
2. Properties Manager opens that object's component stack.
3. A **Scripts** section appears inside Properties Manager.
4. User can search available script-exposed properties/components.
5. User clicks desired property or script component.
6. Property is added/bound immediately and becomes editable.

This flow must be first-class and fast (target: near-instant panel updates).

### Scripts section behavior
- Search by property name, component name, category, or tags.
- Show type metadata (float/int/bool/vector/resource/reference).
- Inline create/edit/remove for script components.
- Validation and tooltip docs on hover.
- Support favorites/recently-used properties for speed.

### Explorer behavior
- Scene/entity hierarchy with drag/drop reparenting.
- Asset browser mode (textures, models, materials, sounds, scripts).
- Filters (by type/tag/path/reference status).
- Dependency and reference view (who uses this asset).

## 4) Integrated Toolset (Internal SDK)

### Editor and content tools
- **World Editor** (Hammer-style): brush/blockout + mesh workflow, entity placement, trigger/event graph wiring
- **Model Compiler + Viewer**: import, skeleton setup, LOD generation, collision proxy preview
- **Animation/Dialogue Staging Tool** (Faceposer-style): facial tracks, lip-sync curves, timeline events
- **Material/Texture Tooling**: texture import, channel packing, compression profiles, material instance editing
- **Distributed Build Worker** (VMPI-style): networked map and lighting build acceleration

### Import and export requirements
- Import: FBX/GLTF/OBJ models, PNG/TGA/TIFF textures, WAV/OGG audio (extendable via plugins).
- Export: cooked game packages, selected asset bundles, and editor-friendly interchange exports.
- Batch import/export with presets.
- Reimport support preserving user overrides when source files change.

### Pipeline expectations
- One-command local compile for maps/materials/models/scripts
- Deterministic outputs for CI reproducibility
- Per-asset validation reports (naming, scale, shader compatibility)
- Fast incremental rebuilds for small changes

## 5) Artist-Driven Iterative Workflow

### Design goals
- Designers and artists can create interactive encounters without touching engine C++.
- Entity I/O links remain first-class for scripted moments.
- Data/script reload should be fast and reliable for iteration.

### Runtime-facing systems
- Entity I/O for trigger chains (buttons, doors, AI state flips, mission beats)
- Material system that drives visuals + physical/audio properties
- Terrain/displacement sculpting tools for rapid environment iteration

### Production guardrails
- Shared naming and folder conventions for cross-title asset reuse
- Asset dependency tracking to avoid accidental hard references to product-only content
- “Override, don’t duplicate” policy for common assets

## 6) Hybrid Physics Strategy

### Split simulation model
- **Simulation physics**: rigid body and environment interaction system for props/world effects
- **Gameplay movement physics**: highly responsive character controller layer for player/NPC movement

### Why hybrid
A pure unified physics stack can reduce responsiveness in high-speed gameplay. A hybrid model preserves feel while keeping world interactions believable.

### Rules of engagement
- Movement controller is authoritative for character locomotion and prediction.
- World simulation owns dynamic props, breakables, and physically-driven set pieces.
- Clear handoff rules are required at interaction boundaries (push, carry, impact, vehicle transitions).

## 7) Shared Asset Ecosystem Across Games

### Mount and precedence model
RCAE titles can mount content from:
1. engine/common packages
2. shared franchise packages
3. title-specific packages
4. dev override packages (non-shipping)

Resolution order should be explicit and logged so teams can debug inherited content quickly.

### Cross-title reuse targets
- Materials and shader presets
- Common props and collision data
- Audio libraries and surface response sets
- Animation rigs and utility behaviors
- Shared texture libraries and utility decals

### Safety controls
- Package manifests define what is exportable vs title-private.
- Build checks fail when private-only assets leak into shared packages.
- Versioned shared assets prevent silent breakage across active projects.

## 8) Suggested Project Hierarchy (Folders)

```text
RCAE/
├─ engine/
│  ├─ core/                     # memory, logging, job system, platform layer (C++)
│  ├─ runtime/
│  │  ├─ ecs/                   # entity/component runtime
│  │  ├─ scene/
│  │  ├─ scripting/             # C# host + bindings layer
│  │  └─ serialization/
│  ├─ rendering/
│  │  ├─ rhi/                   # low-level graphics abstraction
│  │  ├─ renderer/
│  │  ├─ shaders/
│  │  └─ materials/
│  ├─ physics/
│  │  ├─ simulation/            # rigid body/world simulation
│  │  └─ movement/              # gameplay movement controller
│  ├─ audio/
│  ├─ ai/
│  └─ networking/
├─ tools/
│  ├─ editor/                   # main editor app (C# UI + native bridge)
│  │  ├─ panels/
│  │  │  ├─ explorer/
│  │  │  ├─ properties/
│  │  │  └─ console/
│  │  ├─ workflows/
│  │  │  └─ selection_binding/
│  │  └─ plugins/
│  ├─ asset_pipeline/
│  │  ├─ importers/             # fbx, gltf, png, wav ...
│  │  ├─ exporters/
│  │  └─ validators/
│  ├─ build_workers/            # distributed map/light compile workers
│  └─ cli/
├─ shared/
│  ├─ content_common/
│  │  ├─ materials/
│  │  ├─ textures/
│  │  ├─ models/
│  │  ├─ audio/
│  │  └─ scripts/
│  └─ gameplay_common/
├─ games/
│  ├─ game_alpha/
│  │  ├─ content/
│  │  ├─ scripts/
│  │  └─ config/
│  └─ game_beta/
│     ├─ content/
│     ├─ scripts/
│     └─ config/
├─ packages/
│  ├─ manifests/
│  ├─ cooked/
│  └─ overrides_dev/
├─ docs/
└─ tests/
```

## 9) Starter Code Blueprint (Examples)

### 9.1 Engine-side component reflection (C++)
```cpp
// engine/runtime/ecs/ComponentReflection.h
#pragma once
#include <string>
#include <vector>

namespace rcae {

struct PropertyDesc {
    std::string name;
    std::string type;   // float, bool, vec3, asset_ref, etc.
    std::string category;
};

struct ComponentDesc {
    std::string name;
    std::vector<PropertyDesc> properties;
};

class IComponentRegistry {
public:
    virtual ~IComponentRegistry() = default;
    virtual const std::vector<ComponentDesc>& GetAll() const = 0;
};

} // namespace rcae
```

### 9.2 Script component for gameplay logic (C#)
```csharp
// games/game_alpha/scripts/DoorTrigger.cs
using RCAE.Scripting;

public class DoorTrigger : EntityScript
{
    [ExposeProperty("Target Door", "reference")]
    public EntityRef TargetDoor { get; set; }

    [ExposeProperty("Open Delay", "float")]
    public float OpenDelay { get; set; } = 0.25f;

    public override void OnInteract(EntityRef interactor)
    {
        Scheduler.Delay(OpenDelay, () =>
        {
            TargetDoor.Send("Open");
        });
    }
}
```

### 9.3 Editor properties workflow hook (C#)
```csharp
// tools/editor/workflows/selection_binding/SelectionBindingController.cs
public sealed class SelectionBindingController
{
    private readonly ISelectionService _selection;
    private readonly IPropertiesPanel _properties;
    private readonly IScriptCatalog _scriptCatalog;

    public SelectionBindingController(
        ISelectionService selection,
        IPropertiesPanel properties,
        IScriptCatalog scriptCatalog)
    {
        _selection = selection;
        _properties = properties;
        _scriptCatalog = scriptCatalog;

        _selection.Changed += OnSelectionChanged;
    }

    private void OnSelectionChanged(EntityId id)
    {
        var components = _scriptCatalog.GetBindableComponents(id);
        _properties.ShowEntity(id);
        _properties.ShowScriptsSection(components); // searchable + click-to-add
    }
}
```

### 9.4 Asset import manifest (JSON)
```json
{
  "asset": "textures/props/metal_panel_a.png",
  "importer": "texture",
  "settings": {
    "compression": "BC7",
    "srgb": true,
    "generateMipmaps": true,
    "normalMap": false
  },
  "output": "materials/props/metal_panel_a.rmat"
}
```

### 9.5 Package mount order config (YAML)
```yaml
mount_order:
  - engine/common
  - shared/franchise_core
  - games/game_alpha/content
  - packages/overrides_dev
```

## 10) Non-Public Development Note

This architecture direction is intentionally private while internal workflows, editor UX, and compatibility policy are still evolving.

If external sharing is needed later, publish a reduced public-facing architecture brief that omits branch strategy, migration policy details, and internal packaging constraints.

## 11) Source 2 Reference Snapshot (Engine + Development Process)

Source 2 is Valve's modern, data-driven engine architecture and the successor to Source 1. It is used in projects such as Dota 2, Half-Life: Alyx, and Counter-Strike 2, and emphasizes shared tooling, scalability, and high iteration speed across teams.

### Core engine characteristics

- **Modern rendering architecture**
  - Vulkan-first rendering backend
  - Physically based rendering (PBR)
  - Advanced lighting and material workflows
  - Built for large, high-fidelity environments
- **Data-driven and modular design**
  - Systems are designed for reuse across multiple games
  - Shared tooling and asset pipelines reduce per-title duplication
  - Strong separation between engine systems and game-specific logic
- **Multithreaded core**
  - Parallel-by-design runtime architecture
  - Better scaling on modern multi-core CPUs than Source 1-era assumptions
- **Integrated toolchain**
  - Unified editor environment
  - Real-time asset compilation
  - Tight edit -> compile -> test iteration loop

### Development process focus

Valve's Source 2 workflow centers on rapid iteration and common infrastructure:

1. **Unified tools**
   - Projects use common editor and asset tooling
   - Standardized asset formats across projects
   - Tooling upgrades benefit all active products
   - Shared subsystems (for example UI, physics, networking layers) are reusable
2. **Content pipeline**
   - Assets are authored in external DCC tools
   - Content is compiled into optimized runtime formats
   - Data-driven definitions control runtime behavior
3. **Modular game code**
   - Game-specific logic sits on top of shared engine/runtime systems
   - Components can be moved between products when applicable
   - Teams are encouraged to reuse systems rather than duplicate implementations
4. **Fast iteration**
   - Real-time previews inside tools
   - Strong debugging and inspection tooling
   - Workflow designed to minimize compile/test latency

### High-level philosophy

Source 2's value is not only visual fidelity. Its architecture also prioritizes:

- Scalability
- Reusable systems across projects
- Efficient content pipelines
- High-performance multithreaded execution
- Long-term maintainability

## 12) From Architecture Notes to a Functional Engine (Execution Plan)

Short answer: not from documentation alone. A production-ready engine requires an incremental implementation plan, automated validation, and shipping targets. The practical approach is to build a vertical slice, then scale subsystem by subsystem.

### Practical build sequence

1. **Runtime foundation**
   - Main loop and frame pacing
   - Memory/resource ownership model
   - Logging and diagnostics
2. **World model**
   - Entity/component data model
   - Deterministic update ordering
   - Save/load serialization
3. **Content path**
   - Import -> validate -> cook pipeline
   - Stable runtime package format
4. **Rendering + simulation baseline**
   - Minimal renderer with materials and camera
   - Physics/movement integration with test scenes
5. **Tools and iteration**
   - Scene editor bindings
   - Hot-reload for script/content changes
   - Profiling and debugging instrumentation
6. **Production hardening**
   - CI with functional/regression tests
   - Platform matrix and compatibility testing
   - Upgrade/migration policy across titles

### Current repository scope

The current repo now includes a minimal C++ runtime slice (`MiniEngine`) with:
- entity creation
- transform/velocity updates
- collision against a tile map
- frame rendering to text output

This is intentionally a prototype runtime slice, not a complete shipping engine. It is meant to prove core loop behavior and provide a foundation for the next layers (renderer, physics depth, scripting host, editor integration, packaging, and platform services).

## 13) Recommended Split: C# Front-End Workflows + C++ Runtime Back-End

Yes—this is the preferred direction for RCAE iteration speed and runtime performance.

### What should live in C# (front-end/tooling/gameplay layer)
- Editor UX and panels (inspector, explorers, timeline, property editing)
- Gameplay scripting APIs and content-authoring behaviors
- Rapid iteration features (hot reload, validation UI, debugging views)
- Visual workflow orchestration (selection binding, script component wiring)

### What should live in C++ (back-end/runtime layer)
- Frame loop, ECS storage/update, memory/resource systems
- Rendering backend and render graph integration
- Physics simulation/movement authority and deterministic update paths
- Asset streaming, package IO, and platform-specific performance paths

### Interop model
- Keep a narrow native ABI boundary (C ABI) between C# and C++.
- Use C# `DllImport` for tool/runtime bridge calls.
- Keep ownership semantics explicit (who allocates/frees native handles).
- Prefer command-style calls and plain data structs at the boundary.

### Repository implementation status
- Native C++ runtime remains in `MiniEngine`.
- A C ABI wrapper is now available in `MiniEngineCAPI`.
- A C# bridge sample (`MiniEngineNativeBridge.cs`) demonstrates calling native runtime from managed tooling.

## 14) Rendering Baseline (Current Implementation)

To begin Section 4 execution, RCAE now has a concrete renderer baseline with clear responsibilities:

- `Renderer2D` provides deterministic frame composition for sprite instances.
- `Camera2D` defines viewport bounds.
- `Material` and `SpriteInstance` define draw metadata and per-instance layer ordering.
- Draw order is resolved by layer so higher-priority visuals override lower layers at the same pixel.

This establishes a production-oriented rendering seam that can evolve into:
- backend abstraction (Vulkan/other APIs)
- material/shader systems
- scene traversal and batching
- profiling and frame diagnostics

## 15) Post-Rendering Step Implemented: Scene Simulation -> Render Integration

To move beyond isolated rendering, RCAE now includes a `SceneWorld` integration layer that:

- owns scene entities with `Transform2D`, `Velocity2D`, and `Renderable2D`
- runs simulation ticks that advance transforms
- builds render submission instances from world state
- renders through `Renderer2D` via camera-defined view

This forms the first deterministic frame pipeline seam:

1. world state update (`tick`)
2. render instance extraction (`buildRenderInstances`)
3. frame composition (`Renderer2D::renderFrame`)

This is the correct next step after renderer baseline because it connects gameplay state to rendering output in a testable way.

## 16) Movement Authority + Collision Layers (Current Implementation)

`SceneWorld` now includes first-pass gameplay movement authority controls:

- world bounds are authoritative (`setWorldBounds`)
- entity colliders define layer and collision masks
- blocking movement checks occur during `tick` before transform commit

This adds a practical rule set for deterministic movement decisions and establishes the base for future collision channels, triggers, and character controller specialization.

## 17) Axis-Separated Movement and Trigger Events

`SceneWorld` now supports two additional gameplay-runtime behaviors:

- axis-separated movement resolution (X then Y) to allow sliding when only one axis is blocked
- trigger event collection (`consumeTriggerEvents`) for non-blocking gameplay volumes

This keeps deterministic movement authority while adding practical gameplay signaling needed for interactions (checkpoints, doors, scripted beats, etc.).

## 18) Playable Runtime Vertical Slice (Beyond Test-Only Validation)

RCAE now includes a playable CLI runtime app (`rcae_game_app`) that loads authored level data and runs an interactive update loop.

Current vertical-slice behaviors:
- level load from ASCII map files
- player movement commands (W/A/S/D)
- collision against static blockers
- trigger event emission and log output
- live frame rendering after each tick

This moves the repository from test-only validation toward an actual executable gameplay loop that can be iterated and demonstrated.
