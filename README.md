# TESviewer

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

<img src="screenshot.jpg" alt="Main window" width="700"/>

Loads .ESM data files (used in the Elder Scrolls and Fallout franchises) and renders interior/exterior cells using OpenGL. For exterior cells, renders the surrounding land based on the heightmap in an associated LAND record.

Supports a subset of the NIF (NetImmerse) model specification:
* parsing BSVertexDataSSE: vertex/UV/normal coordinates,
* parsing BSLighting(Shader)Property: loading appropriate DDS textures,
* parsing NiAlphaProperty,
* parsing NiAVObject.

In future, a better option than explicit inheritance would be to parse nif.xml specification like NifSkope does.

Uses OpenGL buffers instead of the immediate mode.
