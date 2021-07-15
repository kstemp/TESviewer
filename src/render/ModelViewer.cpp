#include "..\Config.h"
#include "util.h"
#include "ModelViewer.h"

#include <QWheelEvent>
#include <qstatusbar.h>
#include <qtoolbar.h>

ModelViewer::ModelViewer(QWidget* parent)
	: QOpenGLWidget(parent) {
	setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_DeleteOnClose);
}

ModelViewer::~ModelViewer() {
	deleteMeshes();
}

void ModelViewer::addModel(const std::string& modelFileName, const Vector3& globalTranslation, const Vector3& globalRotation, ESM::OBND obnd) {
	// TODO temporary
	if (modelFileName.length() <= 4)
		return;

	NiFile model(SKYRIM_MESHES_DIR + modelFileName);
	model.parse();

	addMesh(model, globalTranslation, globalRotation);

	addBox(obnd, globalTranslation, globalRotation);

	camera.position() = fullBoundingSphere.center.toQVector3D();
	camera.position() += QVector3D(1, 1, 1) * fullBoundingSphere.radius / 300.0f;
	camera.front() = -(fullBoundingSphere.center.toQVector3D() - camera.position()).normalized();
}

void ModelViewer::initializeGL() {
	initializeOpenGLFunctions();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	program.addShaderFromSourceFile(QOpenGLShader::Vertex, VERTEX_SHADER_PATH);
	program.addShaderFromSourceFile(QOpenGLShader::Fragment, FRAGMENT_SHADER_PATH);
	program.link();
}

void ModelViewer::paintGL() {
	glClearColor(0.690, 0.878, 0.902, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);

	// TODO don't recalculate frustum each time ffs
	auto [nr, fr] = getNearFarFrustumFromBoundingSphere(fullBoundingSphere);

	program.bind();

	// TODO also save these matrices somewhere
	QMatrix4x4 projection;
	projection.perspective(zoom, 1.0f * width() / height(), nr, fr);
	program.setUniformValue("projection", projection);

	program.setUniformValue("view", camera.view());

	QMatrix4x4 model;
	model.setToIdentity();
	program.setUniformValue("model", model);

	program.setUniformValue("lightPos", fullBoundingSphere.center.toQVector3D() + QVector3D(1, 0, 0) * phi + QVector3D(0, 1, 0) * phi1 + QVector3D(0, 0, 1) * phi2);
	program.setUniformValue("viewPos", camera.position());

	drawMeshes();
	drawBoxes();
}

void ModelViewer::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
}

void ModelViewer::mouseMoveEvent(QMouseEvent* e) {
	float SPEED = 0.1;

	if (e->buttons() == Qt::LeftButton) {
		float xoffset = -1 * SPEED * (e->x() - lastPos.x());
		float yoffset = -1 * SPEED * (e->y() - lastPos.y());

		camera.addOffsetToYawPitch(xoffset, yoffset);

		lastPos = e->pos();

		update();
	}
}

void ModelViewer::mousePressEvent(QMouseEvent* e) {
	if (e->buttons() == Qt::LeftButton)
		lastPos = e->pos();
}

void ModelViewer::wheelEvent(QWheelEvent* e) {
	zoom += e->angleDelta().y() / 120;
	zoom = std::clamp(zoom, 1.f, 45.f);

	update();
}

void ModelViewer::keyPressEvent(QKeyEvent* e) {
	const float cameraSpeed = fullBoundingSphere.radius / 250.0f;

	switch (e->key()) {
	case Qt::Key::Key_A:
		camera.move(MoveDirection::StrafeLeft);
		break;
	case Qt::Key::Key_S:
		camera.move(MoveDirection::Backward);
		break;
	case Qt::Key::Key_W:
		camera.move(MoveDirection::Forward);
		break;
	case Qt::Key::Key_D:
		camera.move(MoveDirection::StrafeRight);
		break;
	case Qt::Key::Key_R:
		phi += 5.0f;
		break;
	case Qt::Key::Key_Y:
		phi -= 5.0f;
		break;
	case Qt::Key::Key_F:
		phi1 += 5.0f;
		break;
	case Qt::Key::Key_H:
		phi1 -= 5.0f;
		break;
	case Qt::Key::Key_V:
		phi2 += 5.0f;
		break;
	case Qt::Key::Key_N:
		phi2 -= 5.0f;
		break;
	}

	update();
}