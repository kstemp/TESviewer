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

		void parse(BinaryStreamReader& bsr) override {

			while (Record::hasMoreFields(bsr)) {
				std::string fieldName = bsr.readString(4);
				uint16_t fieldSize = bsr.readVar<uint16_t>();

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
				} else
					bsr.skip(fieldSize);

			}

		}

		int getBlock() {
			// last digit of formID in decimal
			return formID % 10;
		}

		int getSubBlock() {
			// penultimate digit of formID in decimal
			return ((formID / 10) % 10);
		}
	};

}