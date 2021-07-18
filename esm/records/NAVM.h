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

	struct NVNM {
		uint32_t unknown;
		uint32_t locationMarker;
		uint32_t worldSpaceFormID;
		uint32_t cellOrGrid;

		int32_t numVerts;
		std::vector<Vector3> vertices;

		int32_t numTris;
		std::vector<Triangle> triangles;
	};

	struct NAVM : Record {
		NAVM() : Record(RecordType::NAVM) {}

		NVNM nvnm;

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) override {
			std::streampos pos__ = bsr.pos;
			if (fieldName == "NVNM") {
				bsr >> nvnm.unknown >> nvnm.locationMarker >> nvnm.worldSpaceFormID >> nvnm.cellOrGrid;

				bsr >> nvnm.numVerts;
				nvnm.vertices.resize(nvnm.numVerts);
				for (int i = 0; i < nvnm.numVerts; ++i)
					nvnm.vertices[i].parse(bsr);

				bsr >> nvnm.numTris;
				nvnm.triangles.resize(nvnm.numTris);
				for (int i = 0; i < nvnm.numTris; ++i) {
					bsr >> nvnm.triangles[i].vertex0 >> nvnm.triangles[i].vertex1 >> nvnm.triangles[i].vertex2;
					bsr.skip(sizeof(int16_t) + 4 * sizeof(uint16_t));
					//bsr >> nvnm.triangles[i].edge0 >> nvnm.triangles[i].edge1 >> nvnm.triangles[i].edge2;
				//	bsr >> nvnm.triangles[i].coverMarker >> nvnm.triangles[i].coverFlags;
				}

				bsr.skip(fieldSize - (bsr.pos - pos__));
			}
			else
				bsr.skip(fieldSize);
		}

		std::string type_pretty() const override {
			return "Navmesh";
		}
	};
}