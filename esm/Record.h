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
		std::string typ = "";
		RecordTypeVal type = 0;
		uint32_t dataSize = 0;
		uint32_t flags = 0;
		uint32_t formID = 0;
		unsigned char vc[4] = { 0, 0, 0, 0 };
		uint16_t version = 0;
		uint16_t unknown = 0;

		std::string EDID = "";

		OBND obnd;

		Record(std::string typ, RecordTypeVal type) : typ(typ), type(type) {}

		const bool hasMoreFields(const BinaryStreamReader& bsr) const {
			return bsr.tellg() - _dataPos < dataSize;
		}

		// TODO is base of and so on
		template<typename T>
		T* castTo() {
			return static_cast<T*>(this);
		}

		template<typename T>
		const T* castTo() const {
			return static_cast<const T*>(this);
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

		virtual void save(BinaryStreamReader& bsr) {
			bsr.
		}

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) = 0;

		virtual std::optional<std::string> model() const {
			return {};
		}

		virtual std::string type_pretty() const = 0;

		std::streampos _dataPos;
	};
}