#pragma once

#include <memory>
#include <qopenglfunctions_3_3_core.h>
#include <QKeyEvent>
#include <QtOpenGLWidgets/qopenglwidget.h>
#include <QOpenGLShaderProgram>
#include <functional>

#include "..\..\esm\Record.h"

#include "..\..\model\NiTypes.h"
#include "..\..\model\NiFile.h"

#include "Camera.h"
#include "Mesh.h"

struct Box {
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;

	const Vector3& globalTranslation;
	const Vector3& globalRotation;

	ESM::OBND bounds;

	Box(const Vector3& globalTranslation, const Vector3& globalRotation) : globalTranslation(globalTranslation), globalRotation(globalRotation) {
	}

	QMatrix4x4 getModelMatrix() const {
		QMatrix4x4 model;

		float angleX = 57.2957795 * globalRotation.x;
		float angleY = 57.2957795 * globalRotation.y;
		float angleZ = 57.2957795 * globalRotation.z;

		model.setToIdentity();

		model.translate(globalTranslation.toQVector3D());
		model.rotate(angleX, QVector3D(1, 0, 0));
		model.rotate(angleY, QVector3D(0, 1, 0));
		model.rotate(angleZ, QVector3D(0, 0, 1));

		return model;
	}
};

class ModelViewer : public QOpenGLWidget, QOpenGLFunctions_3_3_Core {
	Camera camera;

	float phi = 0;
	float phi1 = 0;
	float phi2 = 0;

	QPoint lastPos;

	float zoom = 45.f;

	QOpenGLShaderProgram program;
	QOpenGLShaderProgram normalProgram;

	std::vector<Mesh> meshes;
	std::vector<Box> boxes;

	void addMesh(const NiFile& model, const Vector3& globalTranslation, const Vector3& globalRotation);
	void addBox(const ESM::OBND obnd, const Vector3& globalTranslation, const Vector3& globalRotation);
	void drawMeshes();
	void drawBoxes();
	void deleteMeshes();
	void deleteBoxes() {
		// TODO implement
	}

	NiBound fullBoundingSphere;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

	void mouseMoveEvent(QMouseEvent* e);
	void wheelEvent(QWheelEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void keyPressEvent(QKeyEvent* e);

public:

	ModelViewer(QWidget* parent = Q_NULLPTR);
	~ModelViewer();

	void addModel(const std::string& modelFileName, const Vector3& globalTranslation = Vector3(), const Vector3& globalRotation = Vector3(), ESM::OBND obnd = ESM::OBND());
};