# pammap
[![Build Status](https://travis-ci.org/mfherbst/pammap.svg?branch=master)](https://travis-ci.org/mfherbst/pammap)

Tree-like parameter map for automatically passing parameters from
high-level to low-level conveniently.

## Design principles
- Use `PamMap` as simple hierachical data format
- Single numbers or strings are stored as a copy, lists and larger blocks as views.
- The focus is not on speed, but convenience

## Goal
- Easily read/write PamMap from `python` in Yaml or HDF5
- Easily fill PamMap with data from `python`
- Easily generate documentation about structure of parameter tree
- Easily serialise PamMap into high-level datastructures on `C++` side
- Allow to retrieve PamMap values at low level
- Idea: Low level code is modified and high-level code adapts automatically.

## Requirements
Pammap is known to build if the following requirements are met.
Other versions, especially lower could work as well, but have not been tested.

- cmake 3.0
- C++11 compatible compiler, e.g.
	- clang 5.0
	- gcc 5
