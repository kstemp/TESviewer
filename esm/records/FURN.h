#pragma once

#include "..\Record.h"

namespace ESM {
	namespace RecordType {
		constexpr RecordTypeVal FURN = 45;
	}

	struct FURN : Record {
		std::string MODL;

		FURN() : Record("FURN", RecordType::FURN) {}

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) override {
			if (fieldName == "MODL")
				MODL = bsr.readString(fieldSize);
			else if (fieldName == "EDID")
				EDID = bsr.readString(fieldSize);
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
	};
}