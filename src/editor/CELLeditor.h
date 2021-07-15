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
	ESM::RecordMap& recordMap;

protected:
	void initializeGL() override {
		ModelViewer::initializeGL();

		for (ESM::Record* r : records) {
			switch (r->type) {
			case ESM::RecordType::REFR:
			{
				auto refr = static_cast<ESM::REFR*>(r);

				auto it = recordMap.find(refr->NAME);
				if (it != recordMap.end()) {
					auto base = it->second;

					if (base->model())
						addModel(base->model().value(), refr->DATA.position, refr->DATA.rotation, base->obnd);
				}
			}

			break;
			}
		}
	}

public:

	MyMV2(ESM::RecordList& cellTemporaryChildren,
		ESM::RecordMap& recordMap,
		QWidget* parent)
		:records(cellTemporaryChildren),
		recordMap(recordMap),
		ModelViewer(parent)
	{}
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

	CELLeditor(ESM::CELL* cell, ESM::File& dataFile, QWidget* parent = Q_NULLPTR, std::function<void(int)> onProgress = [](const int) {})
		: QMainWindow(parent), cell(cell), dataFile(dataFile) {
		setWindowTitle(QString::fromStdString("Cell: " + cell->EDID));

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

		//QWidget::connect(modelViewer, &MyMV2::onProgress, this, &CELLeditor::onProgress2);
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