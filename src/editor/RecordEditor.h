#pragma once

#include "AbstractRecordEditor.h"
#include "AbstractRecordDialog.h"

#include <QTableView>
#include <QBoxLayout>

#include "..\model\RecordModel.h"
#include <esm\Util.h>

class RecordEditor : public AbstractRecordDialog, public AbstractRecordEditor {
	Q_OBJECT

		QTableView* tableView;
	RecordModel* recordModel;

public:

	RecordEditor(ESM::Record* record, ESM::File& dataFile, QWidget* parent = Q_NULLPTR)
		: AbstractRecordEditor(record, dataFile), AbstractRecordDialog(parent) {
		const std::string EDID = record->fieldOr<std::string>("EDID", "");

		setWindowTitle(ESM::getRecordFullName(record->type) + ": " + QString::fromStdString(EDID) + (EDID.empty() ? "" : " ") + "[" + NumToHexStr(record->formID) + "]");

		recordModel = new RecordModel(record);

		tableView = new QTableView();
		tableView->setModel(recordModel);

		auto layout = new QHBoxLayout();
		layout->addWidget(tableView);

		setLayout(layout);
	};
};