#pragma once

#include <algorithm>

#include <qtablewidget.h>
#include "ui_CELLeditor.h"

#include <esm\records\CELL.h>
#include <esm\Util.h>
#include <esm\File.h>
#include <esm\Group.h>

#include "..\editor\REFReditor.h"
#include "..\render\CellRenderer.h"
#include "..\model\CellChildrenModel.h"

class CELLeditor : public QMainWindow {
	Q_OBJECT

		ESM::Record* cell;
	ESM::File& dataFile;

	Ui::CELLeditor ui;

	CellRenderer* renderer = nullptr;

	CellChildrenModel* model;

	QDockWidget* dockREFReditor = nullptr;

	void onTableWidgetItemDoubleClicked(QTableWidgetItem* item) {
		uint32_t formID = item->data(Qt::UserRole).toUInt();

		ESM::Record* record = dataFile.findByFormID(formID);

		auto editor = new REFReditor(record, dataFile);

		QWidget::connect(editor, &REFReditor::changed, this, [this]() {
			this->renderer->update();
			});

		dockREFReditor->setWidget(editor);
	}

public:

	CELLeditor(ESM::Record* cell, ESM::File& dataFile, QWidget* parent = Q_NULLPTR, std::function<void(int)> onProgress = [](const int) {})
		: QMainWindow(parent), cell(cell), dataFile(dataFile) {
		ui.setupUi(this);

		setWindowTitle(QString::fromStdString("Cell: " + (*cell)["EDID"].string()));

		dockREFReditor = new QDockWidget("Reference", this);
		dockREFReditor->setAllowedAreas(Qt::RightDockWidgetArea);
		addDockWidget(Qt::RightDockWidgetArea, dockREFReditor);

		//	connect(ui.refTable, &QTableWidget::itemDoubleClicked, this, &CELLeditor::onTableWidgetItemDoubleClicked);

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

		model = new CellChildrenModel(cellTemporaryChildren, dataFile);
		ui.refTable->setModel(model);

		renderer = new CellRenderer(cellTemporaryChildren->records, dataFile, this);

		setCentralWidget(renderer);
	}
};