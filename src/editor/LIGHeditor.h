#pragma once

#include "AbstractRecordEditor.h"
#include "AbstractRecordDialog.h"
#include <esm\File.h>
#include "..\Config.h"
#include "..\util.h"
#include <ui_LIGHeditor.h>

class LIGHeditor : public AbstractRecordDialog, public AbstractRecordEditor {
	Q_OBJECT

		Ui::LIGHeditor ui;

public slots:

	void EDIDChanged(const QString& text) {
		//record.EDID = text.toStdString();
	}

public:

	LIGHeditor(ESM::Record* ligh, ESM::File& dataFile, QWidget* parent = Q_NULLPTR)
		: AbstractRecordEditor(ligh, dataFile), AbstractRecordDialog(parent) {
		ui.setupUi(this);

		ui.editEDID->setText(QString::fromStdString((*record)["EDID"].string()));

		ui.sbFOV->setValue(std::get<float>((*record)["DATA"]["FOV"]));
		ui.sbFalloff->setValue(std::get<float>((*record)["DATA"]["falloff"]));
		ui.sbRadius->setValue(std::get<float>((*record)["DATA"]["radius"]));
		ui.sbNearClip->setValue(std::get<float>((*record)["DATA"]["nearClip"]));

		QWidget::connect(ui.editEDID, &QLineEdit::textChanged, this, &LIGHeditor::EDIDChanged);
	};
};