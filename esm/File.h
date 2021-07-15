#pragma once

#include <vector>

#include "Group.h"
#include "records\TES4.h"

namespace ESM {
	using RecordMap = std::unordered_map<uint32_t, Record*>;
	using RecordList = std::vector<Record*>;

	constexpr int GROUP_COUNT = 118;

	struct File {
		RecordMap recordMap;

		ESM::TES4* header;

		std::vector<Group> groups;

		void parse(const std::string& fileName, const bool headerOnly = false, std::function<void(int, std::string)> onGroupParsed = [](const int, const std::string) {}) {
			auto bsr = BinaryStreamReader(fileName);

			bsr.expect("TES4");

			header = new ESM::TES4();

			header->parseHeader(bsr);
			header->parse(bsr);

			if (!headerOnly) {
				for (int i = 1; i <= GROUP_COUNT; ++i) {
					bsr.expect("GRUP");
					groups.push_back(Group());
					groups.back().parse(bsr, recordMap);

					onGroupParsed(i, groups.back().label);
				}
			}
		}
	};
}