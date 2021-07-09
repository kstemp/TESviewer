#pragma once

#include <qvector3d.h>

#include "..\..\model\NiBound.h"

// corresponds e.g. to an individual BSTriShape
struct SubMesh {
    unsigned int texture = -1;
    int indicesCount = 0;
    int indicesCountUpTo = 0;
    int verticesCount = 0;
    int vertexCountUpTo = 0;

    QVector3D translation; // local - within the model itself
};

// corresponds e.g. to a full NIF model, i.e. all objects under a root BSFadeNode
struct Mesh {

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    std::vector<SubMesh> submeshes;

    NiBound boundingSphere;

    Vector3 globalTranslation;
    Vector3 globalRotation;

    QMatrix4x4 getModelMatrixForSubmesh(const SubMesh& submesh) const {

        QMatrix4x4 model;
        model.setToIdentity();
        model.translate(globalTranslation.toQVector3D());
        model.translate(submesh.translation);
        return model;

    }

};