#include "CK.h"

#include <qfiledialog.h>
#include <qboxlayout.h>
#include <qprogressdialog.h>
#include <QTabWidget>
#include <qprogressdialog.h>

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
	auto fileDialog = new FileDialog();

	fileDialog->exec();

	QProgressDialog progress("Loading skyrim.esm", "Abort", 0, ESM::GROUP_COUNT, this);
	progress.setWindowModality(Qt::WindowModal);

	dataFile.parse(SKYRIM_DATA_DIR + "Skyrim.esm", false, [&](const int groupIndex, const std::string groupName) {
		progress.setValue(groupIndex);
		progress.setLabelText(QString::fromStdString("Loading skyrim.esm [" + groupName + "]"));
		});

	//for (int i = 0; i < numFiles; i++) {
	//if (progress.wasCanceled())
	//	break;
	//... copy one file
//}
	//connect(pd, &QProgressDialog::canceled, this, &Operation::cancel);
	progress.setValue(ESM::GROUP_COUNT);

	populateRecordList();
}

QTreeWidgetItem* CK::getItemFromRecord(const ESM::Record* record) {
	auto item = new QTreeWidgetItem((QTreeWidget*)nullptr/*, {  }*/);

	std::string EDID = "";
	if (record->type == ESM::RecordType::REFR) {
		const ESM::REFR* refr = static_cast<const ESM::REFR*>(record);

		const auto it = dataFile.recordMap.find(refr->NAME);
		if (it != dataFile.recordMap.end()) {
			ESM::Record* base = it->second;

			EDID = "[" + base->EDID + "]";
		}
	}
	else {
		EDID = record->EDID;
	}

	item->setText(0, QString::fromStdString(NumToHexStr(record->formID)));
	item->setText(1, QString::fromStdString(EDID));
	item->setText(2, QString::fromStdString(record->type_pretty()));
	item->setData(0, Qt::UserRole, record->formID);
	item->setData(1, Qt::UserRole, record->formID);
	item->setData(2, Qt::UserRole, record->formID);

	return item;
};

QTreeWidgetItem* CK::loopChildGroups(const ESM::Group& group, const QString& title) {
	QString itemTitle = title.isEmpty() ? QString::fromStdString(group.caption()) : title;
	auto item = new QTreeWidgetItem((QTreeWidget*)nullptr, { itemTitle });

	// all top-level records
	for (const auto& record : group.records)
		item->addChild(getItemFromRecord(record));

	for (const auto& subgroup : group.subgroups)
		item->addChild(loopChildGroups(subgroup));

	return item;
};

void CK::populateRecordList() {
	ui.treeRecords->clear();

	for (const auto& group : dataFile.groups)
		ui.treeRecords->addTopLevelItem(loopChildGroups(group, QString::fromStdString(group.caption())));
}

void CK::onTreeViewItemClicked(QTreeWidgetItem* item, int column) {
	if (!item->data(0, Qt::UserRole).isValid())
		return;

	const uint32_t formID = item->data(0, Qt::UserRole).toInt();

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