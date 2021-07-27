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

class ModelViewer : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core {
protected:

	Camera camera;

	float phi = 0;
	float phi1 = 0;
	float phi2 = 0;

	QPoint lastPos;

	QVector3D lightPos;

	float zoom = 45.f;

	QOpenGLShaderProgram program;
	QOpenGLShaderProgram navMeshProgram;

	QList<QVector3D> lightPositions;
	QList<QVector3D> lightColors;

	std::vector<Mesh> meshes;

	void addMesh(const NiFile& model, const Vector3& globalTranslation, const Vector3& globalRotation);
	void addLight(QVector3D color, QVector3D pos) {
		lightPositions.push_back(pos);
		lightColors.push_back(color);
	};

	void drawMeshes(bool isNavmesh = false);
	void deleteMeshes();

	NiBound fullBoundingSphere;

	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

	void mouseMoveEvent(QMouseEvent* e);
	void wheelEvent(QWheelEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void keyPressEvent(QKeyEvent* e);

public:

	bool doLighting = true;
	bool drawNavmesh = false;
	bool doMeshes = true;

	ModelViewer(QWidget* parent = Q_NULLPTR);
	~ModelViewer();

	void addModel(const std::string& modelFileName, const Vector3& globalTranslation = Vector3(), const Vector3& globalRotation = Vector3(), ESM::OBND obnd = ESM::OBND());
};