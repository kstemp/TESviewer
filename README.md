# ESXedit

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![alt width ="800" height = "640"](video.gif)

An open-source "alternative" (not really) to the CreationKit, a program used to create mods for the Elder Scrolls/Fallout games by Bethesda Game Studios. Made as a hobby project to teach myself OpenGL, and out of curiosity about the elusive .NIF file format.

Supported features:

- [ ] Loading/parsing a .ESM (ES Master) and .ESP (ES Plugin) files.
- [ ] Creating new .ESP plugins, editing existing plugins *and* masters.
- [ ] Loading meshes directly from the BSA archives.
- [ ] Handling alternate textures.
- [ ] Manipulating records (STAT, REFR etc.) with custom-designed forms, saving changes.
- [ ] **Rendering interior cells**, and exterior cells with landscape.
- [ ] Lighting cells, dislaying CreationKit-provided models for light fixtures.
- [ ] Mouse editing of cells/object references.
- [ ] Heightmap editing.
- [ ] **Rendering and manipulating NavMesh** (Navigation Mesh) including dragging vertices, edges, deleting and creating triangles.
- [ ] Editing Papyrus scripts with code completion, runing the compiler and showing compilation error messages (linking to the appropriate line in the code).

<img src="screenshot.jpg" alt="Main window" width="700"/>

Supports a (proper 🤷‍♀️) subset of the NIF (NetImmerse) model specification:
- parsing BSVertexDataSSE: vertex/UV/normal coordinates,
- parsing BSLighting(Shader)Property: loading appropriate DDS textures,
- parsing NiAlphaProperty,
- parsing NiAVObject.

In future, perhaps a better option than explicit inheritance could be to parse nif.xml specification like NifSkope does, and manipulate nodes through some abstract NiNode interface with a std::map of fields. Similarly, ESM records could be represented by some abstract ESM::Record object, but I'm not sure how to handle repeating fields and non-aligned structs. I'm waaaay too lazy to create an interface like the one TES5Edit has, with AbstractUnionDeciders and whatnot. 

Uses OpenGL buffers (instead of the immediate mode, like NifSkope) to efficiently render cells and landscape.
