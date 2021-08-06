#pragma once

#include "AbstractRecordEditor.h"
#include "AbstractRecordDialog.h"

#include <QTableView>
#include <QBoxLayout>

#include "..\model\RecordModel.h"

class RecordEditor : public AbstractRecordDialog, public AbstractRecordEditor {
	Q_OBJECT

		QTableView* tableView;
	RecordModel* recordModel;

public:

	RecordEditor(ESM::Record* record, ESM::File& dataFile, QWidget* parent = Q_NULLPTR)
		: AbstractRecordEditor(record, dataFile), AbstractRecordDialog(parent) {
		recordModel = new RecordModel(record);

		tableView = new QTableView();
		tableView->setModel(recordModel);

		QHBoxLayout* layout = new QHBoxLayout();
		layout->addWidget(tableView);

		setLayout(layout);
	};
};