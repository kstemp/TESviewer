#include "Util.h"

#include "records\CELL.h"

ESM::Record* ESM::getBaseFromREFR(const ESM::Record* refr, const ESM::File& file) {
	const auto it = file.recordMap.find((*refr)["NAME"].get<uint32_t>());

	if (it != file.recordMap.end()) {
		ESM::Record* base = it->second;

		return base;
	}

	return nullptr;
}

std::vector<ESM::Group>* ESM::findCellChildrenTopLevel(const ESM::Record* cell, ESM::File& file) {
	int block = getCellBlock(cell);
	int subBlock = getCellSubBlock(cell);

	std::vector<ESM::Group>* cellChildrenTop = ((*cell)["DATA"].get<uint16_t>() & ESM::CellFlags::Interior) ?
		&file.groups[57].subgroups[block].subgroups[subBlock].subgroups
		: &file.groups[58].subgroups[0].subgroups[0].subgroups;//.subgroups[2].subgroups;

	return cellChildrenTop;
}

ESM::Group* ESM::findCellChildren(ESM::Record* cell, std::vector<ESM::Group>* cellChildrenTop) {
	auto cellChildren = std::find_if(
		cellChildrenTop->begin(),
		cellChildrenTop->end(),
		[&](const auto& g) {
			const uint32_t groupParentFormID = g.labelAsFormID();
			return g.type == ESM::GroupType::CellChildren && groupParentFormID == cell->formID;
		});

	return &(*cellChildren);
}

ESM::Group* ESM::findCellTemporaryChildren(ESM::Record* cell, ESM::Group* cellChildren) {
	auto cellTemporaryChildren = std::find_if(
		cellChildren->subgroups.begin(),
		cellChildren->subgroups.end(),
		[](const auto& g) {
			return g.type == ESM::GroupType::CellTemporaryChildren;
		});

	return &(*cellTemporaryChildren);
}

int ESM::getCellBlock(const ESM::Record* cell) {
	if ((*cell)["DATA"].get<uint16_t>() & ESM::CellFlags::Interior)
		// last digit of formID in decimal
		return cell->formID % 10;
	else
		return 0;
}

int ESM::getCellSubBlock(const ESM::Record* cell) {
	if ((*cell)["DATA"].get<uint16_t>() & ESM::CellFlags::Interior)
		// penultimate digit of formID in decimal
		return ((cell->formID / 10) % 10);
	else
		return 0;
}