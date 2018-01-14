# pammap

Tree-like parameter map for automatically passing parameters from
high-level to low-level conveniently.

## Design principles
- All data is stored in the `PamMap` objects is a copy
- The user either gets copies or views(references) to the data
  stored in the `PamMap`.
- The focus is not on speed, but convenience

## Goal
- Easily read/write PamMap from `python` in Yaml or HDF5
- Easily fill PamMap with data from `python`
- Easily generate documentation about structure of parameter tree
- Easily serialise PamMap into high-level datastructures on `C++` side
- Allow to retrieve PamMap values at low level
- Idea: Low level code is modified and high-level code adapts automatically.

## Requirements
- krims (at the moment)
- C++ ABI
