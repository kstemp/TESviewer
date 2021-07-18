#pragma once

#include <algorithm>
#include <esm\records\CELL.h>
#include "..\editor\REFReditor.h"
#include <esm\File.h>
#include "..\render\CellRenderer.h"

#include <qtablewidget.h>

#include "ui_CELLeditor.h"

class CELLeditor : public QMainWindow {
	Q_OBJECT

		ESM::CELL* cell;
	ESM::File& dataFile;

	Ui::CELLeditor ui;

	CellRenderer* renderer = nullptr;

	QDockWidget* dockREFReditor = nullptr;

	void onTableWidgetItemDoubleClicked(QTableWidgetItem* item) {
		uint32_t formID = item->data(Qt::UserRole).toUInt();

		auto it = dataFile.recordMap.find(formID);
		if (it != dataFile.recordMap.end()) {
			ESM::Record* record = it->second;

			auto editor = new REFReditor(static_cast<ESM::REFR*>(record), dataFile.recordMap);

			QWidget::connect(editor, &REFReditor::changed, this, [this]() {
				this->renderer->update();
				});

			dockREFReditor->setWidget(editor);
		}
	}

public:

	CELLeditor(ESM::CELL* cell, ESM::File& dataFile, QWidget* parent = Q_NULLPTR, std::function<void(int)> onProgress = [](const int) {})
		: QMainWindow(parent), cell(cell), dataFile(dataFile) {
		ui.setupUi(this);

		setWindowTitle(QString::fromStdString("Cell: " + cell->EDID));

		dockREFReditor = new QDockWidget("Reference", this);
		dockREFReditor->setAllowedAreas(Qt::RightDockWidgetArea);
		addDockWidget(Qt::RightDockWidgetArea, dockREFReditor);

		connect(ui.refTable, &QTableWidget::itemDoubleClicked, this, &CELLeditor::onTableWidgetItemDoubleClicked);

		std::vector<ESM::Group>* cellChildrenTop = nullptr;
		cellChildrenTop = (cell->DATA & ESM::CellFlags::Interior) ?
			&dataFile.groups[57].subgroups[cell->getBlock()].subgroups[cell->getSubBlock()].subgroups
			: &dataFile.groups[58].subgroups[0].subgroups[0].subgroups;//.subgroups[2].subgroups;

		const auto cellChildren = std::find_if(
			cellChildrenTop->begin(),
			cellChildrenTop->end(),
			[&](const auto& g) {
				uint32_t groupParentFormID = *(uint32_t*)(&g.label[0]);
				return g.type == ESM::GroupType::CellChildren && groupParentFormID == cell->formID;
			});

		const auto cellTemporaryChildren = std::find_if(
			cellChildren->subgroups.begin(),
			cellChildren->subgroups.end(),
			[](const auto& g) {
				return g.type == ESM::GroupType::CellTemporaryChildren;
			});

		for (ESM::Record* r : cellTemporaryChildren->records) {
			switch (r->type) {
			case ESM::RecordType::REFR:
			{
				auto refr = static_cast<ESM::REFR*>(r);

				auto it = dataFile.recordMap.find(refr->NAME);
				if (it != dataFile.recordMap.end()) {
					ESM::Record* base = it->second;

					auto item1 = new QTableWidgetItem(QString::fromStdString(base->EDID));
					item1->setData(Qt::UserRole, refr->formID);

					auto item2 = new QTableWidgetItem(QString::fromStdString(NumToHexStr(refr->formID)));
					item2->setData(Qt::UserRole, refr->formID);

					auto item3 = new QTableWidgetItem(QString::fromStdString(base->type_pretty()));
					item3->setData(Qt::UserRole, refr->formID);

					ui.refTable->insertRow(ui.refTable->rowCount());
					ui.refTable->setItem(ui.refTable->rowCount() - 1, 0, item1);
					ui.refTable->setItem(ui.refTable->rowCount() - 1, 1, item2);
					ui.refTable->setItem(ui.refTable->rowCount() - 1, 2, item3);
				}
			}
			break;
			}
		}

		renderer = new CellRenderer(cellTemporaryChildren->records, dataFile.recordMap, this);

		setCentralWidget(renderer);
	}
};