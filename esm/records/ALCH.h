#pragma once

#include "..\Record.h"
#include <string>

namespace ESM {
	namespace RecordType {
		constexpr RecordTypeVal ALCH = 5;
	}

	struct ALCH : Record {
		std::string MODL;

		ALCH() : Record("ALCH", RecordType::ALCH) {}

		virtual void saveFields(BinaryStreamWriter& bsw) override {
			bsw.writeField("MODL", MODL);
			bsw.writeField("EDID", EDID);
		}

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) override {
			if (fieldName == "MODL")
				MODL = bsr.readString(fieldSize);
			else if (fieldName == "EDID")
				EDID = bsr.readString(fieldSize);
			else
				bsr.skip(fieldSize);
		}

		std::optional<std::string> model() const override {
			return MODL;
		}

		std::string type_pretty() const override {
			return "Potion";
		}
	};
}