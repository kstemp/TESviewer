#pragma once

#include "Field.h"

#include <optional>
#include <io\BinaryIO.h>

namespace ESM {
	enum RecordFlags {
		Compressed = 0x00040000
	};

	struct Record {
		std::string type;
		uint32_t dataSize = 0;
		uint32_t flags = 0;
		uint32_t formID = 0;
		unsigned char vc[4] = { 0, 0, 0, 0 };
		uint16_t version = 0;
		uint16_t unknown = 0;

		bool modified = false;

		std::vector<Field> fields;

		Record(const std::string& type) : type(type) {}

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

				Field field(fieldName, fieldSize);

				if (fieldSize != 0)
				{
					if (type == "REFR" && fieldName == "DATA") {
						float x, y, z;
						bsr >> x >> y >> z;
						float rx, ry, rz;
						bsr >> rx >> ry >> rz;

						field.struct_("position", "x") = x;
						field.struct_("position", "y") = y;
						field.struct_("position", "z") = z;
						field.struct_("rotation", "x") = rx;
						field.struct_("rotation", "y") = ry;
						field.struct_("rotation", "z") = rz;
					}
					else {
						bsr.readIntoVectorBuf(field.buffer, fieldSize);
					}
				}
				else {
					// If the preceding field has the type XXXX, then dataSize will be 0
					// and the size of the data is in fact the 32 bit quantity stored in the XXXX field.
					// This feature is commonly used to store large navmesh fields in Skyrim.esm.
					if (fields.back().name == "XXXX") {
						uint32_t dataS = fields.back().get<uint32_t>();
						bsr.readIntoVectorBuf(field.buffer, dataS);
					}
				}

				fields.push_back(field);// TODO avoid copying!
			}
		}

		Field& operator [](const std::string& name) {
			auto it = std::find_if(fields.begin(), fields.end(), [&](const ESM::Field& field) {
				return field.name == name;
				});
			// TODO....
			return *it;
		}

		const Field& operator [](const std::string& name) const {
			const auto it = std::find_if(fields.begin(), fields.end(), [&](const ESM::Field& field) {
				return field.name == name;
				});
			// TODO....
			return *it;
		}

		virtual void save(BinaryStreamWriter& bsw) {
			bsw << type;
			_dataSizePos = bsw.os.tellp();
			bsw << dataSize;
			bsw << flags;
			bsw << formID;
			bsw << vc[0] << vc[1] << vc[2] << vc[3];
			bsw << version;
			bsw << unknown;

			_dataPos = bsw.os.tellp();

			//saveFields(bsw);

			std::streampos posAfter = bsw.os.tellp();
			uint32_t dataSize = bsw.os.tellp() - _dataPos;

			bsw.os.seekp(_dataSizePos);

			bsw << dataSize;

			bsw.os.seekp(posAfter);
		}

		std::streampos _dataPos;
		std::streampos _dataSizePos;
	};
}