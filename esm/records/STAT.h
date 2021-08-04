#pragma once

#include "..\Record.h"

namespace ESM {
	enum STATFlags {
		hasTreeLOD = 0x00000040,
		notOnLocalMap = 0x00000200,
		hasDistantLOD = 0x00008000,
		highDefLOD = 0x00020000,
		currents = 0x00080000,
		marker = 0x00800000,
		obstacle = 0x02000000,
		worldMap = 0x10000000,
		navMesh_filter = 0x04000000,
		navMesh_boundingBox = 0x08000000,
		navMesh_ground = 0x40000000
	};
}
/*

struct STAT : Record {
	std::string MODL;

	float maxAngle;
	uint32_t MATOformID;

	STAT() : Record("STAT", RecordType::STAT) {}

	virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) override {
		if (fieldName == "MODL")
			MODL = bsr.readString(fieldSize);
		else if (fieldName == "EDID")
			EDID = bsr.readString(fieldSize);
		else if (fieldName == "DNAM") {
			bsr >> maxAngle;
			bsr >> MATOformID;
		}
		else if (fieldName == "OBND") {
			bsr >> obnd.x1 >> obnd.y1 >> obnd.z1;
			bsr >> obnd.x2 >> obnd.y2 >> obnd.z2;
		}
		else
			bsr.skip(fieldSize);
	}

	std::optional<std::string> model() const override {
		return MODL;
	}

	virtual void saveFields(BinaryStreamWriter& bsw) override {
		bsw.writeField("EDID", EDID);
		//bsw.writeField("XCLC", XCLC);
	}
};
}
*/