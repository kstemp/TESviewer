#pragma once

#include <qtmath>
#include <qvector3d.h>

enum class MoveDirection {
    Forward,
    Backward,
    StrafeRight,
    StrafeLeft
};

class Camera {

    float yaw = -90.0f;
    float pitch = 0.f;

    float speed = 20.;

    QVector3D _position;
    QVector3D _front;
    const QVector3D up;

    QMatrix4x4 _view;

    void recalculateFront() {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        QVector3D newFront;
        newFront.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
        newFront.setZ(sin(qDegreesToRadians(pitch)));
        newFront.setY(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
        _front = newFront;

        recalculateView();
    }

    void recalculateView() {
        _view.setToIdentity();
        _view.lookAt(_position, _position + _front, up);
    }

public:

    Camera(const QVector3D up = { 0.0f, 0.0f, 1.0 }) : up(up) {}

    // TODO horrible name obviously
    void addOffsetToYawPitch(const float yawOffset, const float pitchOffset) {
        yaw += yawOffset;
        pitch += pitchOffset;

        recalculateFront();
    }

    void move(MoveDirection dir) {

        switch (dir) {
        case MoveDirection::Forward:
            _position += _front.normalized() * speed;
            break;
        case MoveDirection::Backward:
            _position -= _front.normalized() * speed;
            break;
        case MoveDirection::StrafeRight:
            _position += QVector3D::crossProduct(_front, up).normalized() * speed;
            break;
        case MoveDirection::StrafeLeft:
            _position -= QVector3D::crossProduct(_front, up).normalized() * speed;
            break;
        }

        recalculateView();

    }

    const QVector3D& position() const {
        return _position;
    }

    QVector3D& position() {
        return _position;
    }

    QVector3D& front() {
        return _front;
    }

    const QMatrix4x4& view() const {
        return _view;
    }
};