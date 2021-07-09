#pragma once

#include <vector>

#include "NiObject.h"
#include "NiTypes.h"

/* Generic node object for grouping. */
struct NiNode : NiAVObject {

	uint numChildren;
	std::vector<ref> children;

	uint numEffects;
	std::vector<ref> effects;

	virtual void parse(BinaryStreamReader& bsr) {

		NiAVObject::parse(bsr);

#ifdef _DEBUG
		OutputDebugStringA("-> NiNode.\n");
#endif

		bsr >> numChildren;
		children.resize(numChildren);
		for (int i = 0; i < numChildren; ++i)
			bsr >> children[i];

		bsr >> numEffects;
		effects.resize(numEffects);
		for (int i = 0; i < numEffects; ++i)
			bsr >> effects[i];

	}
	
};

typedef NiNode BSFadeNode;