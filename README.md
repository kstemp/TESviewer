# TESviewer

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![alt](video.gif)

<img src="screenshot.jpg" alt="Main window" width="700"/>

Loads .ESM data files (used in the Elder Scrolls and Fallout franchises) and renders interior cells with OpenGL.

Supported features:

- rendering interior cells.
- manipulating REFR (reference) record data such as position and rotation with real-time updates.
- displaying a form for STAT (static) record with preview of the NIF model.

Supports a subset of the NIF (NetImmerse) model specification:

- parsing BSVertexDataSSE: vertex/UV/normal coordinates,
- parsing BSLighting(Shader)Property: loading appropriate DDS textures,
- parsing NiAlphaProperty,
- parsing NiAVObject.

In future, a better option than explicit inheritance would be to parse nif.xml specification like NifSkope does.

Uses OpenGL buffers instead of the immediate mode.
