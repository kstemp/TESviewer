#include "CK.h"

#include <qfiledialog.h>
#include <qboxlayout.h>
#include <qprogressdialog.h>
#include <QTabWidget>
#include <qprogressdialog.h>
#include <QtConcurrent/QtConcurrent>
#include "LoaderThread.h"

#include "FileProgressDialog.h"

#include "Config.h"

#include  "util.h"

#include <esm\records\CELL.h>
#include "editor\CELLeditor.h"
#include "editor\EditorResolver.h"
#include "FileDialog.h"

CK::CK(QWidget* parent)
	: QMainWindow(parent) {
	ui.setupUi(this);

	// create actions
	QWidget::connect(ui.actionLoadData, &QAction::triggered, this, &CK::fileOpen);

	auto wrapper = new QWidget();
	wrapper->setLayout(ui.layoutRecords);
	ui.dockRecords->setWidget(wrapper);

	ui.dockRecords->setLayout(ui.layoutRecords);

	connect(ui.treeRecords, &QTreeWidget::itemDoubleClicked, this, &CK::onTreeViewItemClicked);
}

void CK::fileOpen() {
	FileDialog fileDialog;

	const int result = fileDialog.exec();

	if (result == QDialog::Accepted) {
		FileProgressDialog progressDialog;

		//QFutureWatcher<void> futureWatcher;
		//	QObject::connect(&futureWatcher, &QFutureWatcher<void>::finished, &progressDialog, &QDialog::close);
			//QObject::connect(&dialog, &QProgressDialog::canceled, &futureWatcher, &QFutureWatcher<void>::cancel);
			//QObject::connect(&futureWatcher, &QFutureWatcher<void>::progressValueChanged, &progressDialog, &FileProgressDialog::setValue);

		const QSet<QString>& filesToLoad = fileDialog.getFilesToLoad();

		for (const QString& fileName : filesToLoad) {
			dataFiles.push_back(ESM::File(fileName));

			dataFiles.back().parse(SKYRIM_DATA_DIR + fileName.toStdString());
		}
		populateRecordList();
	}
}

QTreeWidgetItem* CK::getItemFromRecord(const ESM::Record* record, const int fileIndex) {
	auto item = new QTreeWidgetItem((QTreeWidget*)nullptr/*, {  }*/);

	std::string EDID = "";
	if (record->type == ESM::RecordType::REFR) {
		const ESM::REFR* refr = static_cast<const ESM::REFR*>(record);

		const auto it = dataFiles[fileIndex].recordMap.find(refr->NAME);
		if (it != dataFiles[fileIndex].recordMap.end()) {
			ESM::Record* base = it->second;

			EDID = "[" + base->EDID + "]";
		}
	}
	else {
		EDID = record->EDID;
	}

	QList<uint32_t> data = { (unsigned int)fileIndex, record->formID };

	item->setText(0, QString::fromStdString(NumToHexStr(record->formID)));
	item->setText(1, QString::fromStdString(EDID));
	item->setText(2, QString::fromStdString(record->type_pretty()));
	item->setData(0, Qt::UserRole, QVariant::fromValue(data));
	item->setData(1, Qt::UserRole, QVariant::fromValue(data));
	item->setData(2, Qt::UserRole, QVariant::fromValue(data));

	return item;
	return nullptr;
};

QTreeWidgetItem* CK::loopChildGroups(const ESM::Group& group, const int fileIndex, const QString& title) {
	QString itemTitle = title.isEmpty() ? QString::fromStdString(group.caption()) : title;
	auto item = new QTreeWidgetItem((QTreeWidget*)nullptr, { itemTitle });

	if (group.type == ESM::GroupType::CellChildren) {
		const auto it = dataFiles[fileIndex].recordMap.find(*(uint32_t*)(&group.label));
		if (it != dataFiles[fileIndex].recordMap.end()) {
			ESM::Record* parentCell = it->second;

			item->setText(1, QString::fromStdString("[" + parentCell->EDID + "]"));
		}
	}

	// all top-level records
	for (const auto& record : group.records)
		item->addChild(getItemFromRecord(record, fileIndex));

	for (const auto& subgroup : group.subgroups)
		item->addChild(loopChildGroups(subgroup, fileIndex));

	return item;
};

void CK::populateRecordList() {
	ui.treeRecords->clear();

	for (int i = 0; i < dataFiles.size(); ++i) {
		const ESM::File& dataFile = dataFiles[i];

		auto item = new QTreeWidgetItem((QTreeWidget*)nullptr, { dataFile.fileName });

		for (const auto& group : dataFile.groups)
			item->addChild(loopChildGroups(group, i, QString::fromStdString(group.caption())));

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

	auto it = dataFile.recordMap.find(formID);
	if (it != dataFile.recordMap.end()) {
		ESM::Record* record = it->second;

		QWidget* editor = nullptr;
		if (record->type == ESM::RecordType::CELL) {
			QProgressDialog progress(QString::fromStdString("Loading cell " + record->EDID), "Abort", 0, 100, this);
			progress.setWindowModality(Qt::WindowModal);

			editor = new CELLeditor(static_cast<ESM::CELL*>(record), dataFile, nullptr);

			progress.setValue(100);
		}
		else
			editor = createEditor(record, dataFile.recordMap);

		if (editor)
			ui.tabEditors->addTab(editor, editor->windowTitle());
	}
}