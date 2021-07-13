#pragma once

#include <algorithm>
#include <esm\records\CELL.h>
#include "..\editor\REFReditor.h"
#include <esm\File.h>
#include "..\render\ModelViewer.h"
#include <qmainwindow.h>
#include <QToolbar>
#include <QDockWidget>
#include <QTableWidget>
#include <QHeaderView>

class MyMV2 : public ModelViewer {
	std::vector<ESM::Record*>& records;
	std::unordered_map<uint32_t, ESM::Record*>& recordMap;
protected:
	void initializeGL() override {
		ModelViewer::initializeGL();

		for (size_t i = 0; i < records.size(); ++i) {
			const auto& r = records[i];

			switch (r->type) {
			case ESM::RecordType::REFR:
			{
				const auto refr = static_cast<ESM::REFR*>(r);

				const auto correspondingRecord = recordMap[refr->NAME];

				if (correspondingRecord)
					if (correspondingRecord->model())
						addModel(correspondingRecord->model().value(), refr->DATA.position, refr->DATA.rotation);
			}
			break;
			}
		}
	}

public:

	MyMV2(std::vector<ESM::Record*>& cellTemporaryChildren, std::unordered_map<uint32_t, ESM::Record*>& recordMap, QWidget* parent) :records(cellTemporaryChildren), recordMap(recordMap), ModelViewer(parent) {}
};

class CELLeditor : public QMainWindow {
	Q_OBJECT

		ESM::CELL* cell;
	ESM::File& dataFile;

	ModelViewer* modelViewer = nullptr;

	QDockWidget* dockREFReditor = nullptr;

	void onTableWidgetItemDoubleClicked(QTableWidgetItem* item) {
		uint32_t formID = item->data(Qt::UserRole).toUInt();

		auto record = dataFile.recordMap[formID]; // TODO replace with find() or sth, because the [] operator will create object for us
		if (!record) // TODO should not happen
			return;

		if (record->type != ESM::RecordType::REFR) // TODO should not happen!
			return;

		auto editor = new REFReditor(static_cast<ESM::REFR*>(record), dataFile.recordMap);

		QWidget::connect(editor, &REFReditor::changed, this, [this]() {
			this->modelViewer->update();
			});

		//editor->show();
		dockREFReditor->setWidget(editor);
	}

public:

	CELLeditor(ESM::CELL* cell, ESM::File& dataFile, QWidget* parent = Q_NULLPTR)
		: QMainWindow(parent), cell(cell), dataFile(dataFile) {
		dockREFReditor = new QDockWidget("Reference", this);
		dockREFReditor->setAllowedAreas(Qt::RightDockWidgetArea);
		addDockWidget(Qt::RightDockWidgetArea, dockREFReditor);
		//dock->setWidget(refTable);

		auto refTable = new QTableWidget();
		refTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		refTable->setColumnCount(3);
		refTable->setHorizontalHeaderLabels({ "Editor ID", "Form ID", "Type" });
		refTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		refTable->horizontalHeader()->setStretchLastSection(true);

		connect(refTable, &QTableWidget::itemDoubleClicked, this, &CELLeditor::onTableWidgetItemDoubleClicked);

		auto dock = new QDockWidget("References", this);
		dock->setAllowedAreas(Qt::RightDockWidgetArea);
		addDockWidget(Qt::RightDockWidgetArea, dock);
		dock->setWidget(refTable);

		std::vector<ESM::Group>* cellChildrenTop = nullptr;
		cellChildrenTop = (cell->DATA & ESM::CellFlags::Interior) ?
			&dataFile.groups[57].subgroups[cell->getBlock()].subgroups[cell->getSubBlock()].subgroups
			: &dataFile.groups[58].subgroups[0].subgroups[1].subgroups[2].subgroups;

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

		for (size_t i = 0; i < cellTemporaryChildren->records.size(); ++i) {
			const auto& r = cellTemporaryChildren->records[i];

			switch (r->type) {
			case ESM::RecordType::REFR:
			{
				const auto refr = static_cast<ESM::REFR*>(r);

				const auto correspondingRecord = dataFile.recordMap[refr->NAME];

				if (correspondingRecord) {
					auto item1 = new QTableWidgetItem(QString::fromStdString(correspondingRecord->EDID));
					item1->setData(Qt::UserRole, refr->formID);

					auto item2 = new QTableWidgetItem(QString::fromStdString(NumToHexStr(refr->formID)));
					item2->setData(Qt::UserRole, refr->formID);

					auto item3 = new QTableWidgetItem(QString::fromStdString(correspondingRecord->type_pretty()));
					item3->setData(Qt::UserRole, refr->formID);

					refTable->insertRow(refTable->rowCount());
					refTable->setItem(refTable->rowCount() - 1, 0, item1);
					refTable->setItem(refTable->rowCount() - 1, 1, item2);
					refTable->setItem(refTable->rowCount() - 1, 2, item3);
				}
			}
			break;
			}
		}

		modelViewer = new MyMV2(cellTemporaryChildren->records, dataFile.recordMap, this);
		setCentralWidget(modelViewer);

		setMinimumSize(1000, 800);

		auto toolbar = new QToolBar();
		this->addToolBar(toolbar);
		toolbar->addAction("Reference list");
		toolbar->addAction("Lighting");
		toolbar->addAction("Navmesh");
		toolbar->setMovable(false);
	}
};