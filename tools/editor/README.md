# RCAE Editor Prototype (C++ Core)

Build and run:

```bash
cmake -S . -B build
cmake --build build
./build/rcae_editor
```

Run tests:

```bash
ctest --test-dir build --output-on-failure
```

The prototype supports:
- Explorer-like entity listing (`entities`)
- Selection + Properties Manager flow (`select`, `inspect`)
- Script search/add for **C++ and C#** components (`scripts`, `addscript`)
- Property editing (`set`)
- Asset import/export (`import`, `export`)
- Mount precedence resolution (tested in C++)
