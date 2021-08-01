#pragma once

#include "..\Record.h"

namespace ESM {
	namespace RecordType {
		constexpr RecordTypeVal CONT = 24;
	}

	struct CONT : Record {
		std::string MODL;

		CONT() : Record("CONT", RecordType::CONT) {}

		virtual void saveFields(BinaryStreamWriter& bsw) override {
			bsw.writeField("MODL", MODL);
			bsw.writeField("EDID", EDID);
			//bsw.writeField("XCLC", XCLC);
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
	};
}