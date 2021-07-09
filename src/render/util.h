#pragma once

#include "..\..\model\NiBound.h"

#include <tuple>

std::tuple<float, float> getNearFarFrustumFromBoundingSphere(const NiBound& bs) {

    float radius = (bs.radius > 1024.0) ? bs.radius : 1024.0;

    float near = fabs(bs.center.z) - radius * 1.5;
    float far = fabs(bs.center.z) + radius * 1.5;

    if (near < 1.0)
        near = 1.0;
    if (far < 2.0)
        far = 2.0;

    if (near > far)
        std::swap(near, far);

    return { near, far };

}