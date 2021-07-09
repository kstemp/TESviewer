#pragma once

#include "NiObject.h"

/* Abstract base class representing all rendering properties. Subclasses are attached to NiAVObjects to control their rendering. */
using NiProperty = NiObjectNET;

/* These are "empty" since they contain fields specific to Fallout 3 and older versions of Nif, which we do not support. */
using NiShaderProperty = NiProperty;

using BSShaderProperty = NiShaderProperty;

/* Bethesda shader property for Skyrim and later. */
struct BSLightingShaderProperty : BSShaderProperty {

	uint flags1;
	uint flags2;

	TexCoord UVoffset;
	TexCoord UVscale;

	ref BSShaderTextureSetRef;

	void parse(BinaryStreamReader& bsr) override {

		NiObjectNET::_isBSLightingShaderProperty = true;
		NiObjectNET::parse(bsr);

#ifdef _DEBUG
		OutputDebugStringA("-> BSLightingShaderProperty.\n");
#endif

		bsr >> flags1 >> flags2;
		UVoffset.parse(bsr);
		UVscale.parse(bsr);

		bsr >> BSShaderTextureSetRef;

	}

};