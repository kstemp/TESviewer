#pragma once

#include "..\Record.h"

namespace ESM {

	namespace RecordType {
		constexpr RecordTypeVal TREE = 113;
	}

	struct TREE : Record {

		std::string MODL;

		TREE() : Record(RecordType::TREE) {}

		void parse(BinaryStreamReader& bsr) override {

			while (Record::hasMoreFields(bsr)) {
				std::string fieldName = bsr.readString(4);
				uint16_t fieldSize = bsr.readVar<uint16_t>();

				if (fieldName == "MODL")
					MODL = bsr.readString(fieldSize);
				else if (fieldName == "EDID")
					EDID = bsr.readString(fieldSize);
				else
					bsr.skip(fieldSize);

			}

		}

		std::optional<std::string> model() const override {
			return MODL;
		}
	};

}