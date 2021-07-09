#pragma once

#include <vector>

#include "NiObject.h"
#include "NiBound.h"
#include "BSVertexDataSSE.h"
#include "Triangle.h"
#include "BSShaderTextureSet.h"

/* (Bethesda-specific) Tri-Shape */
struct BSTriShape : NiAVObject {

	NiBound boundingSphere;

	ref skinRef;
	ref shaderPropertyRef;
	ref alphaPropertyRef;

	BSVertexDesc vertexDesc;

	uint dataSize;

	ushort numVertices;
	std::vector<BSVertexDataSSE> vertexData;

	ushort numTriangles;
	std::vector<Triangle> triangles;

	uint particleDataSize;

	void parse(BinaryStreamReader& bsr) override {

		NiAVObject::parse(bsr);

#ifdef _DEBUG
		OutputDebugStringA("-> BSTriShape.\n");
#endif

		boundingSphere.parse(bsr);

		bsr >> skinRef;
		bsr >> shaderPropertyRef;
		bsr >> alphaPropertyRef;
		bsr >> vertexDesc;
		bsr >> numTriangles;
		bsr >> numVertices;

		bsr >> dataSize;

		uint calculatedDataSize = ((vertexDesc & 0xF) * numVertices * 4) + (numTriangles * 6);
		if (dataSize != calculatedDataSize)
			throw;

		if (dataSize > 0) {
			vertexData.resize(numVertices);
			for (int i = 0; i < numVertices; ++i)
				vertexData[i].parse(bsr, vertexDesc >> 44);

			triangles.resize(numTriangles);
			for (int i = 0; i < numTriangles; ++i)
				triangles[i].parse(bsr);
		}	

		bsr >> particleDataSize;

		// TODO the rest!!!!
		
		/* 
<field name="Particle Data Size" type="uint" calc="(Num Vertices #MUL# 6) #ADD# (Num Triangles #MUL# 3)" vercond="#BS_SSE#"/>
<field name="Particle Vertices" type="HalfVector3" arr1="Num Vertices" cond="Particle Data Size #GT# 0" vercond="#BS_SSE#"/>
<field name="Particle Normals" type="HalfVector3" arr1="Num Vertices" cond="Particle Data Size #GT# 0" vercond="#BS_SSE#"/>
<field name="Particle Triangles" type="Triangle" arr1="Num Triangles" cond="Particle Data Size #GT# 0" vercond="#BS_SSE#"/>
		
		*/

	}

	~BSTriShape() {}

};