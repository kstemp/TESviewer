#include "ModelViewer.h"

#include "..\Config.h"

#include "Mesh.h"
#include "Texture.h"

void ModelViewer::drawMeshes(bool isNavmesh) {
	if (isNavmesh)
	{
		for (const auto& mesh : meshes) {
			if (mesh.isNavmesh) {
				glBindVertexArray(mesh.VAO);

				glLineWidth(1.0);
				navMeshProgram.setUniformValue("color", QVector3D(1.0, 0.0, 0.0));

				//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

				for (const auto& submesh : mesh.submeshes) {
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

					navMeshProgram.setUniformValue("color", QVector3D(0.0, 1.0, 0.0));

					glDrawElements(GL_TRIANGLES, submesh.indicesCount - 3, GL_UNSIGNED_SHORT, (void*)(sizeof(ushort) * 3));
				}

				/*	glLineWidth(3.0f);
					navMeshProgram.setUniformValue("color", QVector3D(0.0, 0.0, 0.0));

					for (const auto& submesh : mesh.submeshes)
						glDrawElements(GL_LINE_LOOP, submesh.indicesCount, GL_UNSIGNED_SHORT, 0);*/

				glBindVertexArray(0);
			}
		}
	}
	else {
		for (const auto& mesh : meshes) {
			if (mesh.isNavmesh) continue;

			glBindVertexArray(mesh.VAO);
			glActiveTexture(GL_TEXTURE0);

			for (const auto& submesh : mesh.submeshes) {
				glBindTexture(GL_TEXTURE_2D, submesh.texture);

				program.setUniformValue("model", mesh.getModelMatrixForSubmesh(submesh));
				program.setUniformValue("model2", mesh.getModel2MatrixForSubmesh(submesh));

				glDrawElementsBaseVertex(GL_TRIANGLES, submesh.indicesCount, GL_UNSIGNED_SHORT, (void*)(submesh.indicesCountUpTo * sizeof(ushort)), submesh.vertexCountUpTo);
			}

			glBindVertexArray(0);
		}
	}
}

void ModelViewer::addMesh(const NiFile& model, const Vector3& globalTranslation, const Vector3& globalRotation) {
	Mesh mesh(globalTranslation, globalRotation);

	glGenVertexArrays(1, &mesh.VAO);
	glGenBuffers(1, &mesh.EBO);
	glGenBuffers(1, &mesh.VBO);

	glBindVertexArray(mesh.VAO);

	int indexCount = 0;     // individual indices
	int vertexCount = 0;    // TODO this counts BSVertexDATA, so change the name!!!
	// compute how many vertices and indices we have so we can allocate the space
	for (auto& obj : model.objects) {
		if (const auto trishape = dynamic_cast<BSTriShape*>(obj.get())) {
			SubMesh submesh;

			submesh.verticesCount = trishape->vertexData.size();
			submesh.vertexCountUpTo = vertexCount;
			vertexCount += trishape->vertexData.size();

			submesh.indicesCount = 3 * trishape->triangles.size();
			submesh.indicesCountUpTo += indexCount;
			indexCount += 3 * trishape->triangles.size();

			mesh.submeshes.push_back(submesh);
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(ushort), NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(BSVertexDataSSE), NULL, GL_STATIC_DRAW);

	int submeshIndex = 0; // TODO pathetic hack
	for (int i = 0; i < model.objects.size(); ++i) {
		if (const auto trishape = dynamic_cast<BSTriShape*>(model.objects[i].get())) {
			Vector3 translation = model.getAVObjectCompoundTranslation(i);
			mesh.submeshes[submeshIndex].translation = translation.toQVector3D();
			mesh.boundingSphere |= NiBound(trishape->boundingSphere.center + translation, trishape->boundingSphere.radius);

			// TODO should probably be "BSShaderProperty"
			if (trishape->shaderPropertyRef >= 0 && trishape->shaderPropertyRef < model.objects.size()) {
				if (auto shaderProp = dynamic_cast<BSLightingShaderProperty*>(model.objects[trishape->shaderPropertyRef].get())) {
					if (auto txtset = dynamic_cast<BSShaderTextureSet*>(model.objects[shaderProp->BSShaderTextureSetRef].get())) {
						try {
							unsigned int texture = loadDDSTexture(context()->functions(), Config::SKYRIM_DATA_DIR + txtset->textures[0]);
							mesh.submeshes[submeshIndex].texture = texture;
						}
						catch (std::exception e) {
							//throw; // TODO deal with this
							mesh.submeshes[submeshIndex].texture = -1;
						}
					}
				}
			}

			// EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, mesh.submeshes[submeshIndex].indicesCountUpTo * sizeof(ushort), trishape->triangles.size() * sizeof(Triangle), trishape->triangles.data());

			// VBO
			glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
			glBufferSubData(GL_ARRAY_BUFFER, mesh.submeshes[submeshIndex].vertexCountUpTo * sizeof(BSVertexDataSSE), trishape->vertexData.size() * sizeof(BSVertexDataSSE), trishape->vertexData.data());

			submeshIndex++;
		}
	}

	// vertex x, y, z coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BSVertexDataSSE), (void*)0);
	glEnableVertexAttribArray(0);

	// texture x, y coordinates
	glVertexAttribPointer(1, 2, GL_HALF_FLOAT, GL_FALSE, sizeof(BSVertexDataSSE), (void*)(offsetof(BSVertexDataSSE, uvx)));
	glEnableVertexAttribArray(1);

	// vertex normal x, y, z coordinates
	glVertexAttribPointer(2, 3, GL_BYTE, GL_FALSE, sizeof(BSVertexDataSSE), (void*)(offsetof(BSVertexDataSSE, normal)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	fullBoundingSphere |= NiBound(mesh.boundingSphere.center + mesh.globalTranslation, mesh.boundingSphere.radius);

	meshes.push_back(mesh);
}

void ModelViewer::deleteMeshes() {
	for (const auto& mesh : meshes) {
		glDeleteVertexArrays(1, &mesh.VAO);
		glDeleteBuffers(1, &mesh.VBO);
		glDeleteBuffers(1, &mesh.EBO);

		for (const auto submesh : mesh.submeshes)
			glDeleteTextures(1, &submesh.texture);
	}
}