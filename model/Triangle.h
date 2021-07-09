#pragma once

#include "NiTypes.h"

/* List of three vertex indices. */
struct Triangle {

	ushort v1;
	ushort v2;
	ushort v3;

	void parse(BinaryStreamReader& bsr) {
		bsr >> v1 >> v2 >> v3;
	}

};