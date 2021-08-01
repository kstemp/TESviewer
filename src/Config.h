#pragma once

#include <string>
#include <qstring.h>

namespace Config {
	const std::string SKYRIM_DATA_DIR = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\";
	const std::string SKYRIM_MESHES_DIR = SKYRIM_DATA_DIR + "meshes\\";

	const QString SHADER_DIR = "shaders\\";

	const QString VERTEX_SHADER_PATH = SHADER_DIR + "vshader.vs";
	const QString GEOMETRY_SHADER_PATH = SHADER_DIR + "gshader.gs";
	const QString FRAGMENT_SHADER_PATH = SHADER_DIR + "fshader.fs";

	const QString NAVM_VERTEX_SHADER_PATH = SHADER_DIR + "navmesh.vs";
	const QString NAVM_FRAGMENT_SHADER_PATH = SHADER_DIR + "navmesh.fs";
}