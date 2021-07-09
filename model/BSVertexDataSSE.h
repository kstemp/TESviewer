#pragma once

#include <array>

#include "NiTypes.h"

#include "..\src\util.h"

struct BSVertexDataSSE {

	Vector3 vertex;
	uint16_t uvx;
	uint16_t uvy;
	ByteVector3 normal;
	//float bitangentX;
	/*
	byte bitangentY;
	ByteVector3 tangent;
	byte bitangentZ;
	ByteColor4 vertexColors;
	std::array<hfloat, 4> boneWeights;
	std::array<byte, 4> boneIndices;
	float eyeData;
	*/
	
	void parse(BinaryStreamReader& bsr, BSVertexDesc arg) {

		if (arg & 0x1)
			vertex.parse(bsr);

		if ((arg & 0x11) == 0x11)
			bsr.skip(4);
			//bsr.readIntoVar(bitangentX);

		// UNUSED if ((arg & 0x11) == 0x1)

		if (arg & 0x2)
			bsr >> uvx >> uvy;

		if (arg & 0x8)
			normal.parse(bsr);

		if (arg & 0x8)
			bsr.skip(1);
		//readIntoVar(out.bitangentY, fs);

		if ((arg & 0x18) == 0x18)
			bsr.skip(3);
		//readByteVector(out.tangent, fs);

		if ((arg & 0x18) == 0x18)
			bsr.skip(1);
		//readIntoVar(out.bitangentZ, fs);

		if (arg & 0x20)
			bsr.skip(4);
		//readByteColor4(out.byteColor, fs);

		if (arg & 0x40)
			bsr.skip(4 * 2);
		//readIntoVar(out.byteColor, fs); 4 bone weights!!!

		if (arg & 0x40)
			bsr.skip(4 * 1);
		//readIntoVar(out.byteColor, fs); 4 bytes

		if (arg & 0x100)
			bsr.skip(4);

	}

}; 