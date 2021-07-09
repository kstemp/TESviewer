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

		REFR() : Record(RecordType::REFR) {}

		void parse(BinaryStreamReader& bsr) override {

			while (Record::hasMoreFields(bsr)) {
				std::string fieldName = bsr.readString(4);
				uint16_t fieldSize = bsr.readVar<uint16_t>();

				if (fieldName == "NAME")
					bsr >> NAME;
				else if (fieldName == "DATA") {
					DATA.position.parse(bsr);
					DATA.rotation.parse(bsr);
				} else if (fieldName == "EDID")
					EDID = bsr.readString(fieldSize);
				else 
					bsr.skip(fieldSize);

			}

		}
	};

}