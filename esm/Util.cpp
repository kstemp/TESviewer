#include "Util.h"

#include "records\CELL.h"

ESM::Record* ESM::getBaseFromREFR(const ESM::Record* refr, ESM::File& file) {
	return file.findByFormID(refr->fieldOr<uint32_t>("NAME"));
}

std::vector<ESM::Group>* ESM::findCellChildrenTopLevel(const ESM::Record* cell, ESM::File& file) {
	int block = getCellBlock(cell);
	int subBlock = getCellSubBlock(cell);

	std::vector<ESM::Group>* cellChildrenTop = (cell->fieldOr<uint16_t>("DATA") & ESM::CellFlags::Interior) ?
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
	if (cell->fieldOr<uint16_t>("DATA") & ESM::CellFlags::Interior)
		// last digit of formID in decimal
		return cell->formID % 10;
	else
		return 0;
}

int ESM::getCellSubBlock(const ESM::Record* cell) {
	if (cell->fieldOr<uint16_t>("DATA") & ESM::CellFlags::Interior)
		// penultimate digit of formID in decimal
		return ((cell->formID / 10) % 10);
	else
		return 0;
}

QString ESM::getRecordFullName(const std::string& name) {
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
	else if (name == "NAVM")
		return "Navigation Mesh";
	return QString::fromStdString(name);
}

QString ESM::getGroupCaption(const ESM::Group& group) {
	switch (group.type) {
	case ESM::GroupType::Top:
		return getRecordFullName(group.label);
		break;

	case  ESM::GroupType::WorldChildren:
		return "World Children";
		break;

	case ESM::GroupType::InteriorCellBlock:
		return "Block " + QString::number(*(int32_t*)(&group.label[0]));
		break;

	case ESM::GroupType::InteriorCellSubBlock:
		return "Sub-Block " + QString::number(*(int32_t*)(&group.label[0]));
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

	default:
		return "Group"; // TODO should not happen?
		break;
	}
}