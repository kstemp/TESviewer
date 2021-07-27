#pragma once

#include <algorithm>
#include <esm\records\CELL.h>
#include <esm\Util.h>
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

		ESM::Record* record = dataFile.findByFormID(formID);

		auto editor = new REFReditor(record->castTo<ESM::REFR>(), dataFile);

		QWidget::connect(editor, &REFReditor::changed, this, [this]() {
			this->renderer->update();
			});

		dockREFReditor->setWidget(editor);
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

		QWidget::connect(ui.actionSetNavmeshMode, &QAction::triggered, this, [=]() {
			renderer->drawNavmesh = !renderer->drawNavmesh;
			renderer->update();
			});

		ui.actionEnableLights->setChecked(true);
		QWidget::connect(ui.actionEnableLights, &QAction::triggered, this, [=]() {
			renderer->doLighting = ui.actionEnableLights->isChecked();
			renderer->update();
			});

		ui.actionDrawMeshes->setChecked(true);
		QWidget::connect(ui.actionDrawMeshes, &QAction::triggered, this, [=]() {
			renderer->doMeshes = ui.actionDrawMeshes->isChecked();
			renderer->update();
			});

		std::vector<ESM::Group>* cellChildrenTop = ESM::findCellChildrenTopLevel(cell, dataFile);

		ESM::Group* cellChildren = ESM::findCellChildren(cell, cellChildrenTop);

		ESM::Group* cellTemporaryChildren = ESM::findCellTemporaryChildren(cell, cellChildren);

		for (ESM::Record* r : cellTemporaryChildren->records) {
			switch (r->type) {
			case ESM::RecordType::REFR:
			{
				auto refr = r->castTo<ESM::REFR>();
				ESM::Record* base = ESM::getBaseFromREFR(refr, dataFile);

				if (base) {
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

		renderer = new CellRenderer(cellTemporaryChildren->records, dataFile, this);

		setCentralWidget(renderer);
	}
};