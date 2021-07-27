#pragma once

#include "..\Record.h"

namespace ESM {
	namespace RecordType {
		constexpr RecordTypeVal CELL = 18;
	}

	enum CellFlags {
		Interior = 0x0001
	};

	struct CELL : Record {
		uint16_t DATA;

		struct {
			int32_t X;
			int32_t Y;
			uint32_t flags;
		} XCLC;

		CELL() : Record("CELL", RecordType::CELL) {}

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) override {
			if (fieldName == "EDID")
				EDID = bsr.readString(fieldSize);
			else if (fieldName == "XCLC") {
				bsr >> XCLC.X >> XCLC.Y >> XCLC.flags;
			}
			else if (fieldName == "DATA") {
				if (fieldSize == 2)
					bsr >> DATA;
				else if (fieldSize == 1) {
					char temp;
					bsr >> temp;
					DATA = *(uint16_t*)(&temp);
				}
			}
			else
				bsr.skip(fieldSize);
		}

		int getBlock() const {
			if (DATA & ESM::CellFlags::Interior)
				// last digit of formID in decimal
				return formID % 10;
			else
				return 0;
		}

		int getSubBlock() const {
			if (DATA & ESM::CellFlags::Interior)
				// penultimate digit of formID in decimal
				return ((formID / 10) % 10);
			else
				return 0;
		}

		std::string type_pretty() const override {
			return "Cell";
		}
	};
}