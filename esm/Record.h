#pragma once

#include <optional>
#include <io\BinaryIO.h>

namespace ESM {

	typedef uint32_t RecordTypeVal;

	enum RecordFlags {
		Compressed = 0x00040000
	};

	struct Record {

		RecordTypeVal type	= 0;
		uint32_t dataSize	= 0;
		uint32_t flags		= 0;
		uint32_t formID		= 0;
		unsigned char vc[4] = { 0, 0, 0, 0 };
		uint16_t version	= 0;
		uint16_t unknown	= 0;

		std::string EDID = "";

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

		virtual void parse(BinaryStreamReader& bsr) = 0;

		virtual std::optional<std::string> model() const {
			return {};
		}

		std::streampos _dataPos;

	};

}