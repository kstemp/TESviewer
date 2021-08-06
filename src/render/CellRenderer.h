#pragma once

#include "ModelViewer.h"

class CellRenderer : public ModelViewer {
	std::vector<ESM::Record*>& records;
	ESM::File& dataFile;

public:

	void addNavmesh(ESM::Record* navm) {
		/*	Mesh mesh;
			mesh.isNavmesh = true;

			glGenVertexArrays(1, &mesh.VAO);
			glGenBuffers(1, &mesh.EBO);
			glGenBuffers(1, &mesh.VBO);

			glBindVertexArray(mesh.VAO);

			SubMesh submesh;

			submesh.verticesCount = navm->NVNM.numVerts;

			submesh.indicesCount = 3 * navm->NVNM.numTris;

			mesh.submeshes.push_back(submesh);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, submesh.indicesCount * sizeof(ushort), navm->NVNM.triangles.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
			glBufferData(GL_ARRAY_BUFFER, submesh.verticesCount * 3 * sizeof(float), navm->NVNM.vertices.data(), GL_STATIC_DRAW);

			// vertex x, y, z coordinates
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);

			meshes.push_back(mesh);(*/
	}

	void initializeGL() override {
		ModelViewer::initializeGL();

		for (ESM::Record* r : records) {
			if (r->type == "REFR") {
				ESM::Record* refr = r;

				ESM::Record* base = ESM::getBaseFromREFR(refr, dataFile);

				if (base) {
					for (auto f : base->fields) {
						if (f.name == "MODL") {
							std::string model = f.string();

							float x = std::get<float>((*refr)["DATA"]("position", "x"));
							float y = std::get<float>((*refr)["DATA"]("position", "y"));
							float z = std::get<float>((*refr)["DATA"]("position", "z"));
							float rx = std::get<float>((*refr)["DATA"]("rotation", "x"));
							float ry = std::get<float>((*refr)["DATA"]("rotation", "y"));
							float rz = std::get<float>((*refr)["DATA"]("rotation", "z"));

							addModel(model, Vector3(x, y, z), Vector3(rx, ry, rz));

							/*if (base->type == "LIGH") {
								ESM::Record* ligh = base;

								float r = std::get<float>((*ligh)["DATA"].struct_["r"]);
								float g = std::get<float>((*ligh)["DATA"].struct_["g"]);
								float b = std::get<float>((*ligh)["DATA"].struct_["b"]);

								/*float px = std::get<float>((*refr)["DATA"].struct_("position", "x"));
								float py = std::get<float>((*refr)["DATA"].struct_("position", "y"));
								float pz = std::get<float>((*refr)["DATA"].struct_("position", "z"));

								addLight(QVector3D(r / 255.0f, g / 255.0f, b / 255.0f), refr->DATA.position.toQVector3D());(
							}*/
						}
					}
				}
			}
			else if (r->type == "NAVM") {
				addNavmesh(r);
			}
		}
	}

public:

	CellRenderer(ESM::RecordList& cellTemporaryChildren,
		ESM::File& dataFile,
		QWidget* parent)
		:records(cellTemporaryChildren),
		dataFile(dataFile),
		ModelViewer(parent)
	{}
};