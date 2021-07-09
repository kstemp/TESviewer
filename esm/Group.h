#pragma once

#include "..\src\util.h"

#include <io\BinaryIO.h>

#include "Record.h"
#include "records\STAT.h"
#include "records\REFR.h"
#include "records\MISC.h"
#include "records\CELL.h"
#include "records\ALCH.h"
#include "records\FURN.h"
#include "records\CONT.h"
#include "records\FLOR.h"
#include "records\TREE.h"
#include "records\LAND.h"

namespace ESM {

	enum GroupType {
		Top = 0,
		CellChildren = 6,
		CellTemporaryChildren = 9
	};

	inline Record* createRecordByName(const std::string& name) {

		if (name == "STAT")
			return new STAT();
		else if (name == "REFR")
			return new REFR();
		else if (name == "MISC")
			return new MISC();
		else if (name == "CELL")
			return new CELL();
		else if (name == "ALCH")
			return new ALCH();
		else if (name == "FURN")
			return new FURN();
		else if (name == "CONT")
			return new CONT();
		else if (name == "FLOR")
			return new FLOR();
		else if (name == "TREE")
			return new TREE();
		else if (name == "LAND")
			return new LAND();
		else
			return nullptr;
	}

	struct Group {

		uint32_t size;
		char label[4];
		uint32_t type;
		std::array<unsigned char, 4> vcinfo;
		uint32_t unknown;

		std::vector<Record*> records;
		std::vector<Group> subgroups;

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
				} else {

					Record* newRecord = createRecordByName(hdr);

					if (newRecord) {

						_parseRecord(newRecord, bsr);

						records.push_back(newRecord);
						recordMap[newRecord->formID] = newRecord;

					} else {
						bsr.skip(bsr.readVar<uint32_t>() + 16);;
					}

				}

			}

		}

	private:

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
			} else {
				newRecord->parse(bsr);
			}

		}

	};

}