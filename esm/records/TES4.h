#pragma once

#include "..\Record.h"

namespace ESM {
	namespace RecordType {
		constexpr RecordTypeVal TES4 = 113;
	}

	struct TES4 : Record {
		std::vector<std::string> MAST;
		std::string CNAM = "";
		std::string SNAM = "";

		TES4() : Record("TES4", RecordType::TES4) {}

		virtual void saveFields(BinaryStreamWriter& bsw) override {
			for (const std::string& master : MAST)
				bsw.writeField("MAST", master);

			//bsw.writeField("XCLC", XCLC);
		}

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) override {
			if (fieldName == "MAST")
				MAST.push_back(bsr.readString(fieldSize));
			else if (fieldName == "CNAM")
				CNAM = bsr.readString(fieldSize);
			else if (fieldName == "SNAM")
				SNAM = bsr.readString(fieldSize);
			else
				bsr.skip(fieldSize);
		}

		std::string type_pretty() const override {
			return "TES4 header";
		}
	};
}