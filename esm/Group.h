#pragma once

#include <array>

#include <io\BinaryIO.h>
#include "..\src\util.h"
#include "Record.h"

namespace ESM {
	enum GroupType {
		Top = 0,
		WorldChildren = 1,
		InteriorCellBlock = 2,
		InteriorCellSubBlock = 3,
		ExteriorCellBlock = 4,
		ExteriorCellSubBlock = 5,
		CellChildren = 6,
		TopicChildren = 7,
		CellPersistentChildren = 8,
		CellTemporaryChildren = 9
	};

	struct Group {
		uint32_t size;
		char label[4];
		uint32_t type;
		std::array<unsigned char, 4> vcinfo;
		uint32_t unknown;

		std::vector<Record*> records;
		std::vector<Group> subgroups;

		uint32_t labelAsFormID() const {
			return *(uint32_t*)(&label[0]);
		}

		void parse(BinaryStreamReader& bsr, std::unordered_map<uint32_t, Record*>& recordMap) {
			bsr >> size;
			bsr >> label[0] >> label[1] >> label[2] >> label[3];
			bsr >> type;
			bsr.skip(4);
			bsr >> unknown;

			auto _dataPos = bsr.tellg();

			while ((bsr.tellg() - _dataPos) < size - 24) {
				auto hdr = bsr.readString(4);

				if (hdr == "GRUP") {
					subgroups.push_back(Group());
					subgroups.back().parse(bsr, recordMap);
				}
				else {
					Record* newRecord = new Record(hdr);

					if (newRecord) {
						_parseRecord(newRecord, bsr);

						records.push_back(newRecord);
						recordMap[newRecord->formID] = newRecord;
					}
					else {
						bsr.skip(bsr.readVar<uint32_t>() + 16);;
					}
				}
			}
		}

	private:

		// TODO record itself should do that? MOVE!
		void _parseRecord(Record* newRecord, BinaryStreamReader& bsr) {
			newRecord->parseHeader(bsr);

			if (newRecord->flags & RecordFlags::Compressed) {
				uint32_t decompSize = bsr.readVar<uint32_t>();

				std::vector<char> compressedData;
				bsr.readIntoVectorBuf(compressedData, newRecord->dataSize - 4);

				std::vector<char> decompressedData;

				if (!decompressVectorBuf(decompressedData, decompSize, compressedData, newRecord->dataSize - 4))
					throw;

				BinaryStreamReader newbsr(decompressedData);

				newRecord->dataSize = decompSize;
				newRecord->parse(newbsr);
			}
			else {
				newRecord->parse(bsr);
			}
		}
	};
}