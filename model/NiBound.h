#pragma once

#include "NiTypes.h"
#include "..\io\BinaryIO.h"

/* A sphere. */
struct NiBound {

	Vector3 center;
	float radius;

	void parse(BinaryStreamReader& bsr) {
		center.parse(bsr);
		bsr >> radius;
	}

	NiBound(Vector3 center = { 0, 0, 0 }, float radius = 0) : center(center), radius(radius) {};

	NiBound& operator |= (const NiBound& rhs) {

		if (rhs.radius < 0)
			return *this;

		if (radius < 0)
			return operator=(rhs);

		float d = (center - rhs.center).length();

		if (radius >= d + rhs.radius)
			return *this;

		if (rhs.radius >= d + radius)
			return operator=(rhs);

		if (rhs.radius > radius)
			radius = rhs.radius;

		radius += d / 2;
		center = (center + rhs.center);

		center.x /= 2;
		center.y /= 2;
		center.z /= 2;

		return *this;
	}
};