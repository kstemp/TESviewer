#pragma once

#include "AbstractRecordEditor.h"
#include "AbstractRecordDialog.h"
#include <esm\records\LIGH.h>
#include "..\Config.h"
#include "..\util.h"
#include <ui_LIGHeditor.h>

class LIGHeditor : public AbstractRecordDialog, public AbstractRecordEditor<ESM::LIGH> {
	Q_OBJECT

		Ui::LIGHeditor ui;

public slots:

	void EDIDChanged(const QString& text) {
		record.EDID = text.toStdString();
	}

public:

	LIGHeditor(ESM::LIGH* ligh, ESM::File& dataFile, QWidget* parent = Q_NULLPTR)
		: AbstractRecordEditor(ligh, dataFile), AbstractRecordDialog(parent) {
		ui.setupUi(this);

		ui.editEDID->setText(QString::fromStdString(record.EDID));

		ui.sbFOV->setValue(record.data.FOV);
		ui.sbFalloff->setValue(record.data.falloff);
		ui.sbRadius->setValue(record.data.radius);
		ui.sbNearClip->setValue(record.data.nearClip);

		QWidget::connect(ui.editEDID, &QLineEdit::textChanged, this, &LIGHeditor::EDIDChanged);
	};
};