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
#include "records\MATO.h"
#include "records\LIGH.h"
#include "records\NAVM.h"

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
		else if (name == "MATO")
			return new MATO();
		else if (name == "LIGH")
			return new LIGH();
		else if (name == "NAVM")
			return new NAVM();
		else
			return nullptr;
	}

	inline std::string getRecordFullName(const std::string& name) {
		if (name == "CELL")
			return "Cell";
		else if (name == "TES4")
			return "TES Header";
		else if (name == "WRLD")
			return "Worldspace";
		else if (name == "STAT")
			return "Static";
		else if (name == "FURN")
			return "Furniture";
		else if (name == "CONT")
			return "Container";
		else if (name == "DOOR")
			return "Door";
		else if (name == "LIGH")
			return "Light";
		else if (name == "MISC")
			return "Miscellanous Object";
		else if (name == "ALCH")
			return "Potion";
		else if (name == "FLOR")
			return "Flora";
		else if (name == "MATO")
			return "Material Object";
		else if (name == "TREE")
			return "Tree";
		return name;
	}

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
					Record* newRecord = createRecordByName(hdr);

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

		std::string caption() const {
			switch (type) {
			case ESM::GroupType::Top:
				return getRecordFullName(label);
				break;

			case  ESM::GroupType::WorldChildren:
				return "World Children";
				break;

			case ESM::GroupType::InteriorCellBlock:
				return "Block " + std::to_string(*(int32_t*)(&label[0]));
				break;

			case ESM::GroupType::InteriorCellSubBlock:
				return "Sub-Block " + std::to_string(*(int32_t*)(&label[0]));
				break;

			case ESM::GroupType::ExteriorCellBlock:
				return "Block";
				break;
			case ESM::GroupType::ExteriorCellSubBlock:
				return "Sub-Block";
				break;
			case ESM::GroupType::CellChildren:
				return "Cell children";
				break;
			case ESM::GroupType::TopicChildren:
				return "Topic children";
				break;
			case ESM::GroupType::CellPersistentChildren:
				return "Persistent";
				break;
			case ESM::GroupType::CellTemporaryChildren:
				return "Temporary";
				break;
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