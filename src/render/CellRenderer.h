#pragma once

#include "ModelViewer.h"

class CellRenderer : public ModelViewer {
	std::vector<ESM::Record*>& records;
	ESM::File& dataFile;

public:

	void addNavmesh(ESM::NAVM* navm) {
		Mesh mesh;
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

		meshes.push_back(mesh);
	}

	void initializeGL() override {
		ModelViewer::initializeGL();

		for (ESM::Record* r : records) {
			switch (r->type) {
			case ESM::RecordType::REFR:
			{
				auto refr = r->castTo<ESM::REFR>();

				ESM::Record* base = ESM::getBaseFromREFR(refr, dataFile);

				if (base) {
					if (base->model())
						addModel(base->model().value(), refr->DATA.position, refr->DATA.rotation, base->obnd);

					if (base->type == ESM::RecordType::LIGH) {
						ESM::LIGH* ligh = base->castTo<ESM::LIGH>();

						addLight(QVector3D(ligh->data.r / 255.0f, ligh->data.g / 255.0f, ligh->data.b / 255.0f), refr->DATA.position.toQVector3D());
					}
				}
			}
			break;
			case ESM::RecordType::NAVM:

				addNavmesh(r->castTo<ESM::NAVM>());

				break;
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