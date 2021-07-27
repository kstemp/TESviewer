#pragma once

#include "File.h"
#include "Group.h"
#include "Record.h"
#include "records\REFR.h"
#include "records\CELL.h"

namespace ESM {
	ESM::Record* getBaseFromREFR(const ESM::REFR* refr, const ESM::File& file);

	std::vector<ESM::Group>* findCellChildrenTopLevel(const ESM::CELL* cell, ESM::File& file);

	ESM::Group* findCellChildren(ESM::CELL* cell, std::vector<ESM::Group>* cellChildrenTop);

	ESM::Group* findCellTemporaryChildren(ESM::CELL* cell, ESM::Group* cellChildren);
}
