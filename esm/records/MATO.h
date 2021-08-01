#pragma once

#include "..\Record.h"

namespace ESM {
	namespace RecordType {
		constexpr RecordTypeVal MATO = 72;
	}

	struct MATO : Record {
		std::string MODL;

		MATO() : Record("MATO", RecordType::MATO) {}

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
	};
}