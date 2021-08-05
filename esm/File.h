#pragma once

#include <vector>
#include <QString>

#include "Group.h"

namespace ESM {
	using RecordMap = std::unordered_map<uint32_t, Record*>;
	using RecordList = std::vector<Record*>;

	constexpr int GROUP_COUNT = 118;

	struct File {
		RecordMap recordMap;

		ESM::Record header = ESM::Record("TES4");

		QString fileName;

		std::vector<Group> groups;

		File(const QString& fileName) : fileName(fileName) {
		}

		void parse(const std::string& fileName, const bool headerOnly = false) {
			auto bsr = BinaryStreamReader(fileName);

			bsr.expect("TES4");

			header.parseHeader(bsr);
			header.parse(bsr);

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

			header.save(bsw);

			for (auto [key, record] : recordMap) {
				if (record->modified)
					record->save(bsw);
			}

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