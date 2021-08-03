#pragma once

#include "..\Record.h"
#include <string>
#include <model\NiTypes.h>

namespace ESM {
	namespace RecordType {
		constexpr RecordTypeVal NAVM = 82;
	}

	struct Triangle {
		int16_t vertex0, vertex1, vertex2;
		//	int16_t edge0;
	//		uint16_t edge1, edge2;
		//	uint16_t coverMarker;
			//uint16_t coverFlags;
	};

	struct NAVM : Record {
		NAVM() : Record("NAVM", RecordType::NAVM) {}

		struct {
			uint32_t unknown;
			uint32_t locationMarker;
			uint32_t worldSpaceFormID;
			uint32_t cellOrGrid;

			int32_t numVerts;
			std::vector<Vector3> vertices;

			int32_t numTris;
			std::vector<Triangle> triangles;
		} NVNM;

		virtual void saveFields(BinaryStreamWriter& bsw) override {
			//bsw.writeField("XCLC", XCLC);
		}

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) override {
			std::streampos pos__ = bsr.pos;
			if (fieldName == "NVNM") {
				bsr >> NVNM.unknown >> NVNM.locationMarker >> NVNM.worldSpaceFormID >> NVNM.cellOrGrid;

				bsr >> NVNM.numVerts;
				NVNM.vertices.resize(NVNM.numVerts);
				for (int i = 0; i < NVNM.numVerts; ++i)
					NVNM.vertices[i].parse(bsr);

				bsr >> NVNM.numTris;
				NVNM.triangles.resize(NVNM.numTris);
				for (int i = 0; i < NVNM.numTris; ++i) {
					bsr >> NVNM.triangles[i].vertex0 >> NVNM.triangles[i].vertex1 >> NVNM.triangles[i].vertex2;
					bsr.skip(sizeof(int16_t) + 4 * sizeof(uint16_t));
					//bsr >> nvnm.triangles[i].edge0 >> nvnm.triangles[i].edge1 >> nvnm.triangles[i].edge2;
				//	bsr >> nvnm.triangles[i].coverMarker >> nvnm.triangles[i].coverFlags;
				}

				bsr.skip(fieldSize - (bsr.pos - pos__));
			}
			else
				bsr.skip(fieldSize);
		}
	};
}