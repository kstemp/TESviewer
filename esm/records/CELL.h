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

		CELL() : Record(RecordType::CELL) {}

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) override {
			if (fieldName == "EDID")
				EDID = bsr.readString(fieldSize);
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

		int getBlock() {
			// last digit of formID in decimal
			return formID % 10;
		}

		int getSubBlock() {
			// penultimate digit of formID in decimal
			return ((formID / 10) % 10);
		}

		std::string type_pretty() override {
			return "Cell";
		}
	};
}