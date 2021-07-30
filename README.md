# TESviewer

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![alt width ="800" height = "640"](video.gif)

An open-source "alternative" (not really) to the CreationKit, a program used to create mods for the Elder Scrolls/Fallout games by Bethesda Game Studios. Made as a hobby project to teach myself OpenGL, and out of curiosity about the elusive .NIF file format.

Supported features:

- [ ] loading/parsing a .ESM (ES Master) and .ESP (ES Plugin) files
- [ ] creating new .ESP plugins, editing existing plugins *and* masters
- [ ] **displaying .NIF models** associated with markers, statics, furniture, lights etc. 
- [ ] loading meshes directly from the BSA archives
- [ ] handling alternate textures
- [ ] manipulating records (STAT, REFR etc.) with custom-designed forms, saving changes
- [ ] **rendering interior cells**, and exterior cells with landscape
- [ ] lighting cells, dislaying CreationKit-provided models for light fixtures
- [ ] mouse editing of cells/object references
- [ ] heightmap editing
- [ ] **rendering and manipulating NavMesh** (Navigation Mesh) including dragging vertices, edges, deleting and creating triangles

<img src="screenshot.jpg" alt="Main window" width="700"/>

Supports a (proper 🤷‍♀️) subset of the NIF (NetImmerse) model specification:
- parsing BSVertexDataSSE: vertex/UV/normal coordinates,
- parsing BSLighting(Shader)Property: loading appropriate DDS textures,
- parsing NiAlphaProperty,
- parsing NiAVObject.

In future, perhaps a better option than explicit inheritance could be to parse nif.xml specification like NifSkope does, and manipulate nodes through some abstract NiNode interface with a std::map of fields. Similarly, ESM records could be represented by some abstract ESM::Record object, but I'm not sure how to handle repeating fields and non-aligned structs. I'm waaaay too lazy to create an interface like the one TES5Edit has, with AbstractUnionDeciders and whatnot. 

Uses OpenGL buffers (instead of the immediate mode, like NifSkope) to efficiently render cells and landscape.
