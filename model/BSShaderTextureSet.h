#pragma once

#include <vector>
#include <string>

#include "NiObject.h"
#include "NiTypes.h"

/* Bethesda-specific Texture Set */
struct BSShaderTextureSet : NiObject {

	uint numTextures;
	std::vector<std::string> textures;

	void parse(BinaryStreamReader& bsr) {

#ifdef _DEBUG
		OutputDebugStringA("Parsing BSShaderTextureSet.\n");
#endif

		bsr >> numTextures;
		textures.resize(numTextures);
		for (int i = 0; i < numTextures; ++i)
			textures[i] = readSizedString(bsr);

	}

};