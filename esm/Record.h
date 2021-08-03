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

		bool modified = false;

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

		virtual void save(BinaryStreamWriter& bsw) {
			bsw << typ;
			_dataSizePos = bsw.os.tellp();
			bsw << dataSize;
			bsw << flags;
			bsw << formID;
			bsw << vc[0] << vc[1] << vc[2] << vc[3];
			bsw << version;
			bsw << unknown;

			_dataPos = bsw.os.tellp();

			saveFields(bsw);

			std::streampos posAfter = bsw.os.tellp();
			uint32_t dataSize = bsw.os.tellp() - _dataPos;

			bsw.os.seekp(_dataSizePos);

			bsw << dataSize;

			bsw.os.seekp(posAfter);
		}

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) = 0;
		virtual void saveFields(BinaryStreamWriter& bsw) = 0;

		virtual std::optional<std::string> model() const {
			return {};
		}

		std::streampos _dataPos;
		std::streampos _dataSizePos;
	};
}