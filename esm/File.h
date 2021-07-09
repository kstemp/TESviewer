#pragma once

#include <vector>

#include "Group.h"

namespace ESM {

	struct File {

		std::unordered_map<uint32_t, Record*> recordMap;

		std::vector<Group> groups;

		void parse(const std::string& fileName) {

			auto bsr = BinaryStreamReader(fileName);

			bsr.expect("TES4");

			bsr.skip(3 * 16 + 14 + 12);

			for (int i = 1; i <= 118; ++i) {

				bsr.expect("GRUP");
				groups.push_back(Group());
				groups.back().parse(bsr, recordMap);

			}

		}
	};

}