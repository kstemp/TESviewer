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

	NiFile model(Config::SKYRIM_MESHES_DIR + modelFileName);
	model.parse();

	addMesh(model, globalTranslation, globalRotation);

	camera.position() = fullBoundingSphere.center.toQVector3D();
	camera.position() += QVector3D(1, 1, 1) * fullBoundingSphere.radius / 300.0f;
	camera.front() = -(fullBoundingSphere.center.toQVector3D() - camera.position()).normalized();

	lightPos = camera.position();
}

void ModelViewer::initializeGL() {
	initializeOpenGLFunctions();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program.addShaderFromSourceFile(QOpenGLShader::Vertex, Config::VERTEX_SHADER_PATH);
	program.addShaderFromSourceFile(QOpenGLShader::Fragment, Config::FRAGMENT_SHADER_PATH);
	program.link();

	navMeshProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, Config::NAVM_VERTEX_SHADER_PATH);
	navMeshProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, Config::NAVM_FRAGMENT_SHADER_PATH);
	navMeshProgram.link();
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

	program.setUniformValue("doLighting", doLighting);

	program.setUniformValue("lightColor", lightColors[0]);
	program.setUniformValue("lightColor2", lightColors[1]);
	program.setUniformValue("lightColor3", lightColors[2]);
	program.setUniformValue("lightColor4", lightColors[3]);
	program.setUniformValue("lightColor5", lightColors[4]);
	program.setUniformValue("lightColor6", lightColors[5]);
	program.setUniformValue("lightColor7", lightColors[6]);

	QMatrix4x4 model;
	model.setToIdentity();
	program.setUniformValue("model", model);

	program.setUniformValue("lightPos", lightPos[0]);
	program.setUniformValue("lightPos2", lightPos[1]);
	program.setUniformValue("lightPos3", lightPos[2]);
	program.setUniformValue("lightPos4", lightPos[3]);
	program.setUniformValue("lightPos5", lightPos[4]);
	program.setUniformValue("lightPos6", lightPos[5]);
	program.setUniformValue("lightPos7", lightPos[6]);

	//	program.setUniformValue("viewPos", camera.position());

	if (doMeshes) {
		drawMeshes(false);
	}

	if (drawNavmesh) {
		navMeshProgram.bind();
		navMeshProgram.setUniformValue("model", model);
		navMeshProgram.setUniformValue("projection", projection);
		navMeshProgram.setUniformValue("view", camera.view());
		navMeshProgram.setUniformValue("viewPos", camera.position());

		drawMeshes(true);
	}
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
	else {
		const QPoint center = rect().center();
		const QVector2D dir = QVector2D(e->pos() - center);

		const auto ray = camera.front() + QVector3D(dir.x(), dir.y(), 0);

		std::stringstream p;
		p << "x: " << ray.x() << " y: " << ray.y() << " z: " << ray.z() << "\n";
		OutputDebugStringA(p.str().c_str());
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
	default:
		break;
	}

	update();
}