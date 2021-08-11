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
						bsr.readIntoVectorBuf(field("position", "x").buf, sizeof(float));
						bsr.readIntoVectorBuf(field("position", "y").buf, sizeof(float));
						bsr.readIntoVectorBuf(field("position", "z").buf, sizeof(float));
						bsr.readIntoVectorBuf(field("rotation", "x").buf, sizeof(float));
						bsr.readIntoVectorBuf(field("rotation", "y").buf, sizeof(float));
						bsr.readIntoVectorBuf(field("rotation", "z").buf, sizeof(float));
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

		template <typename T>
		const T fieldOr(const std::string& name, const T defaultVal = T(0)) const {
			auto it = std::find_if(fields.begin(), fields.end(), [&](const ESM::Field& field) {
				return field.name == name;
				});

			if (it == fields.end())
				return defaultVal;
			else
				return it->get<T>();
		}

		template <>
		const std::string fieldOr(const std::string& name, const std::string defaultVal) const {
			auto it = std::find_if(fields.begin(), fields.end(), [&](const ESM::Field& field) {
				return field.name == name;
				});

			if (it == fields.end())
				return defaultVal;
			else
				return it->string();
		}

		Field& operator [](const std::string& name) {
			const auto it = std::find_if(fields.begin(), fields.end(), [&](const ESM::Field& field) {
				return field.name == name;
				});

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