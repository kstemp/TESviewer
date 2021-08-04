#pragma once

#include "File.h"
#include "Group.h"
#include "Record.h"

namespace ESM {
	ESM::Record* getBaseFromREFR(const ESM::Record* refr, const ESM::File& file);

	std::vector<ESM::Group>* findCellChildrenTopLevel(const ESM::Record* cell, ESM::File& file);

	ESM::Group* findCellChildren(ESM::Record* cell, std::vector<ESM::Group>* cellChildrenTop);

	ESM::Group* findCellTemporaryChildren(ESM::Record* cell, ESM::Group* cellChildren);

	int getCellBlock(const ESM::Record* cell);

	int getCellSubBlock(const ESM::Record* cell);
}
