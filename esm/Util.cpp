#include "Util.h"

ESM::Record* ESM::getBaseFromREFR(const ESM::REFR* refr, const ESM::File& file) {
	const auto it = file.recordMap.find(refr->NAME);
	if (it != file.recordMap.end()) {
		ESM::Record* base = it->second;

		return base;
	}

	return nullptr;
}

std::vector<ESM::Group>* ESM::findCellChildrenTopLevel(const ESM::CELL* cell, ESM::File& file) {
	std::vector<ESM::Group>* cellChildrenTop = (cell->DATA & ESM::CellFlags::Interior) ?
		&file.groups[57].subgroups[cell->getBlock()].subgroups[cell->getSubBlock()].subgroups
		: &file.groups[58].subgroups[0].subgroups[0].subgroups;//.subgroups[2].subgroups;

	return cellChildrenTop;
}

ESM::Group* ESM::findCellChildren(ESM::CELL* cell, std::vector<ESM::Group>* cellChildrenTop) {
	auto cellChildren = std::find_if(
		cellChildrenTop->begin(),
		cellChildrenTop->end(),
		[&](const auto& g) {
			const uint32_t groupParentFormID = g.labelAsFormID();
			return g.type == ESM::GroupType::CellChildren && groupParentFormID == cell->formID;
		});

	return &(*cellChildren);
}

ESM::Group* ESM::findCellTemporaryChildren(ESM::CELL* cell, ESM::Group* cellChildren) {
	auto cellTemporaryChildren = std::find_if(
		cellChildren->subgroups.begin(),
		cellChildren->subgroups.end(),
		[](const auto& g) {
			return g.type == ESM::GroupType::CellTemporaryChildren;
		});

	return &(*cellTemporaryChildren);
}