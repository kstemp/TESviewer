# TESviewer

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![alt](video.gif)

<img src="screenshot.jpg" alt="Main window" width="700"/>

Loads .ESM data files (used in the Elder Scrolls and Fallout franchises) and renders interior cells with OpenGL.

Supported features:

[] loading/parsing a .ESM (ES Master) and .ESP (ES Plugin) files
[] creating new .ESP plugins, editing existing plugins *and* masters
[] **displaying .NIF models** associated with markers, statics, furniture, lights etc. 
[] manipulating records (STAT, REFR etc.) with custom-designed forms, saving changes
[] **rendering interior cells**, and exterior cells with landscape
[] mouse editing of cells/object references
[] heightmap editing
[] **rendering and manipulating NavMesh** (Navigation Mesh) including dragging vertices, edges, deleting and creating triangles

Supports a (proper 🤷‍♀️) subset of the NIF (NetImmerse) model specification:
- parsing BSVertexDataSSE: vertex/UV/normal coordinates,
- parsing BSLighting(Shader)Property: loading appropriate DDS textures,
- parsing NiAlphaProperty,
- parsing NiAVObject.

In future, a better option than explicit inheritance could be to parse nif.xml specification like NifSkope does. U

ses OpenGL buffers (instead of the immediate mode, like NifSkope) to efficiently render cells and landscape.
