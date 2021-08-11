#include "CK.h"

#include <qfiledialog.h>
#include <qboxlayout.h>
#include <qprogressdialog.h>
#include <QTabWidget>
#include <qprogressdialog.h>
#include <QtConcurrent/QtConcurrent>

#include "Config.h"

#include  "util.h"

#include <esm\records\CELL.h>
#include <esm\Util.h>
#include "editor\CELLeditor.h"
#include "editor\EditorResolver.h"
#include "FileDialog.h"

CK::CK(QWidget* parent)
	: QMainWindow(parent) {
	ui.setupUi(this);

	QWidget::connect(ui.actionLoadData, &QAction::triggered, this, &CK::fileOpen);
	QWidget::connect(ui.actionSave, &QAction::triggered, this, &CK::fileSave);
	QWidget::connect(ui.actionSaveAs, &QAction::triggered, this, &CK::fileSaveAs);

	auto wrapper = new QWidget();
	wrapper->setLayout(ui.layoutRecords);
	ui.dockRecords->setWidget(wrapper);

	ui.dockRecords->setLayout(ui.layoutRecords);

	connect(ui.treeRecords, &QTreeWidget::itemDoubleClicked, this, &CK::onTreeViewItemClicked);

	//model = new RecordTreeModel(dataFiles);
	//ui.treeRecords->setModel(model);
}

void CK::fileOpen() {
	FileDialog fileDialog;

	const int result = fileDialog.exec();

	if (result == QDialog::Accepted) {
		const QSet<QString>& filesToLoad = fileDialog.getFilesToLoad();

		size_t i = 0;
		for (const QString& fileName : filesToLoad) {
			dataFiles.push_back(ESM::File(fileName));
			dataFiles.back().parse(Config::SKYRIM_DATA_DIR + fileName.toStdString(), false);
			QString ac = fileDialog.getActiveFile();

			if (fileDialog.getActiveFile() == fileName)
				activeFileIndex = i;

			i++;
		}

		populateRecordList();
		setWindowTitle((activeFileIndex >= 0 ? dataFiles[activeFileIndex].fileName : "[unnamed plugin]") + " - TESeditor");
	}
}

void CK::fileSave() {
	if (activeFileIndex >= 0)
		dataFiles[activeFileIndex].save();
	else
		fileSaveAs();
}

void CK::fileSaveAs() {
	QFileDialog dialog;
	QString fileName = dialog.getSaveFileName(this, "Select name for the plugin file", QString::fromStdString(Config::SKYRIM_DATA_DIR), "*.esp");

	if (fileName.isEmpty())
		return;

	dataFiles.push_back(ESM::File(fileName));
	activeFileIndex = dataFiles.size() - 1;

	fileSave();
}

QTreeWidgetItem* CK::getItemFromRecord(const ESM::Record* record, const int fileIndex) {
	auto item = new QTreeWidgetItem((QTreeWidget*)nullptr/*, {  }*/);

	std::string EDID = "";
	if (record->type == "REFR") {
		const ESM::Record* base = ESM::getBaseFromREFR(record, dataFiles[fileIndex]);

		if (base)
			EDID = "[" + base->fieldOr<std::string>("EDID", "") + "]";
	}
	else {
		EDID = record->fieldOr<std::string>("EDID", "");
	}

	QList<uint32_t> data = { (unsigned int)fileIndex, record->formID };

	item->setText(0, NumToHexStr(record->formID));
	item->setText(1, QString::fromStdString(EDID));
	item->setText(2, ESM::getRecordFullName(record->type));
	item->setData(0, Qt::UserRole, QVariant::fromValue(data));
	item->setData(1, Qt::UserRole, QVariant::fromValue(data));
	item->setData(2, Qt::UserRole, QVariant::fromValue(data));

	return item;
};

QTreeWidgetItem* CK::loopChildGroups(const ESM::Group& group, const int fileIndex, const QString& title) {
	QString itemTitle = title.isEmpty() ? ESM::getGroupCaption(group) : title;
	auto item = new QTreeWidgetItem((QTreeWidget*)nullptr, { itemTitle });

	if (group.type == ESM::GroupType::CellChildren)
		if (ESM::Record* parentCell = dataFiles[fileIndex].findByFormID(*(uint32_t*)(&group.label)))
			item->setText(1, QString::fromStdString("[" + parentCell->fieldOr<std::string>("EDID", "") + "]"));

	// all top-level records
	for (const auto& record : group.records)
		item->addChild(getItemFromRecord(record, fileIndex));

	for (const auto& subgroup : group.subgroups)
		item->addChild(loopChildGroups(subgroup, fileIndex));

	return item;
};

void CK::populateRecordList() {
	//ui.treeRecords->update();
	ui.treeRecords->clear();

	for (int i = 0; i < dataFiles.size(); ++i) {
		const ESM::File& dataFile = dataFiles[i];

		auto item = new QTreeWidgetItem((QTreeWidget*)nullptr, { dataFile.fileName });

		for (const auto& group : dataFile.groups)
			item->addChild(loopChildGroups(group, i, ESM::getGroupCaption(group)));

		ui.treeRecords->addTopLevelItem(item);
	}
}

void CK::onTreeViewItemClicked(QTreeWidgetItem* item, int column) {
	if (!item->data(0, Qt::UserRole).isValid())
		return;

	QVariantList data = item->data(0, Qt::UserRole).toList();

	const uint32_t fileIndex = data[0].toUInt();
	const uint32_t formID = data[1].toUInt();

	ESM::File& dataFile = dataFiles[fileIndex];

	ESM::Record* record = dataFile.findByFormID(formID);

	if (record) {
		QWidget* editor = nullptr;
		if (record->type == "CELL") {
			QProgressDialog progress(QString::fromStdString("Loading cell " + (*record)["EDID"].string()), "Abort", 0, 100, this);
			progress.setWindowModality(Qt::WindowModal);

			editor = new CELLeditor(record, dataFile, nullptr);

			progress.setValue(100);
		}
		else
			editor = createEditor(record, dataFile);

		if (editor)
			ui.tabEditors->addTab(editor, editor->windowTitle());
	}
}