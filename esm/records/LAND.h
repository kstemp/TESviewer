#pragma once

#include <array>
#include "..\Record.h"

namespace ESM {
	namespace RecordType {
		constexpr RecordTypeVal LAND = 62;
	}

	constexpr uint16_t VHGT_SIZE = 1096;
	constexpr int LAND_SIZE = 33;

	struct LAND : Record {
		std::array<std::array<float, LAND_SIZE>, LAND_SIZE> heightmap;

		LAND() : Record(RecordType::LAND) {}

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) override {
			if (fieldName == "VHGT") {
				assert(fieldSize == VHGT_SIZE);
				_parseHeightmap(bsr);
				bsr.skip(3);
			}
			else
				bsr.skip(fieldSize);
		}

		std::string type_pretty() override {
			return "Landscape";
		}

	private:

		// adapted from: https://en.uesp.net/wiki/Tes5Mod:Mod_File_Format/LAND
		void _parseHeightmap(BinaryStreamReader& bsr) {
			// whole cell offset each unit equals 8 game units
			float offset = bsr.readVar<float>() * 8;
			float row_offset = 0;

			for (int i = 0; i < 1089; i++) {
				// signed byte, each unit equals 8 game units
				float value = bsr.readVar<char>() * 8;

				int r = i / 33;
				int c = i % 33;

				// first column value controls height for all remaining points in cell
				if (c == 0) {
					row_offset = 0;
					offset += value;
				}
				else {					// other col values control height of all points in the same row
					row_offset += value;
				}

				heightmap[c][r] = offset + row_offset;
			}
		}
	};
}