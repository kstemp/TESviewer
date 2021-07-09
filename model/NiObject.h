#pragma once

#include <vector>
#include <Windows.h>

#include "..\io\BinaryIO.h"

#include "NiTypes.h"

/* Abstract object type. */
struct NiObject {
	virtual void parse(BinaryStreamReader& bsr) = 0;
	virtual ~NiObject() = 0;
};

inline NiObject::~NiObject() { }

/* Abstract base class for NiObjects that support names, extra data, and time controllers. */
struct NiObjectNET : NiObject {

	uint shaderType; // TODO  should be BSLightingShaderType, as enum 
	ref nameStringRef; // TODO dedicated stringRef typedef for uint?
	uint numExtraDataList;

	std::vector<uint> extraData;

	ref controller;
	 
	virtual void parse(BinaryStreamReader& bsr) override {

#ifdef _DEBUG
		OutputDebugStringA("Parsing NiObjectNET ");
#endif

		if (_isBSLightingShaderProperty)
			bsr >> shaderType;

		bsr >> nameStringRef;

		bsr >> numExtraDataList;
		extraData.resize(numExtraDataList);
		for (int i = 0; i < numExtraDataList; ++i)
			bsr >> extraData[i];

		bsr >> controller;

	}

	~NiObjectNET() {}

protected:
	bool _isBSLightingShaderProperty = false;
};

/* Abstract audio-visual base class from which all of Gamebryo's scene graph objects inherit. */
struct NiAVObject : NiObjectNET {

	uint flags;
	Vector3 translation;
	Matrix33 rotation;
	float scale;
	ref collisionObjectRef; 

	virtual void parse(BinaryStreamReader& bsr) override {

		NiObjectNET::parse(bsr);

#ifdef _DEBUG
		OutputDebugStringA("-> NiAVObject ");
#endif

		bsr >> flags;
		translation.parse(bsr);
		rotation.parse(bsr);
		bsr >> scale;
		bsr >> collisionObjectRef;

	}

	~NiAVObject() {}
};