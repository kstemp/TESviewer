#pragma once

#include <model\NiTypes.h>
#include "..\Record.h"

namespace ESM {
	namespace RecordType {
		constexpr RecordTypeVal REFR = 92;
	}

	struct REFR : Record {
		uint32_t NAME;

		struct {
			Vector3 position;
			Vector3 rotation;
		} DATA;

		REFR() : Record("REFR", RecordType::REFR) {}

		virtual void saveFields(BinaryStreamWriter& bsw) override {
			bsw.writeField("EDID", EDID);

			//bsw.writeField("XCLC", XCLC);
		}

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) override {
			if (fieldName == "NAME")
				bsr >> NAME;
			else if (fieldName == "DATA") {
				DATA.position.parse(bsr);
				DATA.rotation.parse(bsr);
			}
			else if (fieldName == "EDID")
				EDID = bsr.readString(fieldSize);
			else
				bsr.skip(fieldSize);
		}
	};
}