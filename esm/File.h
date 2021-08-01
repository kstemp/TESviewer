#pragma once

#include <vector>
#include <QString>

#include "Group.h"
#include "records\TES4.h"

namespace ESM {
	using RecordMap = std::unordered_map<uint32_t, Record*>;
	using RecordList = std::vector<Record*>;

	constexpr int GROUP_COUNT = 118;

	struct File {
		RecordMap recordMap;

		ESM::TES4* header;

		QString fileName;

		std::vector<Group> groups;

		File(const QString& fileName) : fileName(fileName) {
			header = new ESM::TES4();
		}

		void parse(const std::string& fileName, const bool headerOnly = false) {
			auto bsr = BinaryStreamReader(fileName);

			bsr.expect("TES4");

			//header = new ESM::TES4();

			header->parseHeader(bsr);
			header->parse(bsr);

			if (!headerOnly) {
				while (bsr.pos < bsr.fileSize) {
					bsr.expect("GRUP");
					groups.push_back(Group());
					groups.back().parse(bsr, recordMap);
				}
			}
		}

		void save() {
			auto bsw = BinaryStreamWriter(fileName.toStdString());

			header->save(bsw);

			bsw.os.close();
		}

		ESM::Record* findByFormID(const uint32_t formID) {
			auto it = recordMap.find(formID);
			if (it != recordMap.end()) {
				ESM::Record* record = it->second;
				return record;
			}

			return nullptr;
		}
	};
}