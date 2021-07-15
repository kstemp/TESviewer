#pragma once

#include <optional>
#include <io\BinaryIO.h>

namespace ESM {
	typedef uint32_t RecordTypeVal;

	enum RecordFlags {
		Compressed = 0x00040000
	};

	struct OBND {
		int16_t x1 = 0;
		int16_t y1 = 0;
		int16_t z1 = 0;
		int16_t x2 = 0;
		int16_t y2 = 0;
		int16_t z2 = 0;
	};

	struct Record {
		RecordTypeVal type = 0;
		uint32_t dataSize = 0;
		uint32_t flags = 0;
		uint32_t formID = 0;
		unsigned char vc[4] = { 0, 0, 0, 0 };
		uint16_t version = 0;
		uint16_t unknown = 0;

		std::string EDID = "";

		OBND obnd;

		Record(RecordTypeVal type) : type(type) {}

		const bool hasMoreFields(const BinaryStreamReader& bsr) const {
			return bsr.tellg() - _dataPos < dataSize;
		}

		virtual void parseHeader(BinaryStreamReader& bsr) {
			bsr >> dataSize;
			bsr >> flags;
			bsr >> formID;
			bsr >> vc[0] >> vc[1] >> vc[2] >> vc[3];
			bsr >> version;
			bsr >> unknown;

			if (flags & RecordFlags::Compressed)
				_dataPos = 0;
			else
				_dataPos = bsr.tellg();
		}

		void parse(BinaryStreamReader& bsr) {
			while (Record::hasMoreFields(bsr)) {
				std::string fieldName = bsr.readString(4);
				uint16_t fieldSize = bsr.readVar<uint16_t>();

				parseField(bsr, fieldName, fieldSize);
			}
		}

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) = 0;

		virtual std::optional<std::string> model() const {
			return {};
		}

		virtual std::string type_pretty() const = 0;

		std::streampos _dataPos;
	};
}