#pragma once

#include <QDialog>
#include "AbstractRecordEditor.h"
#include "AbstractRecordDialog.h"
#include <ui_QUSTeditor.h>

#include "..\util.h"
#include "..\Config.h"

class QUSTeditor : public AbstractRecordDialog, public AbstractRecordEditor {
	Q_OBJECT

		Ui::QUSTeditor ui;

public:

	QUSTeditor(ESM::Record* qust, ESM::File& dataFile, QWidget* parent = Q_NULLPTR)
		: AbstractRecordEditor(qust, dataFile), AbstractRecordDialog(parent) {
		ui.setupUi(this);
	};
};