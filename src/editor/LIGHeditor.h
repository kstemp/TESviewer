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

public:

	LIGHeditor(ESM::LIGH* ligh, ESM::File& dataFile, QWidget* parent = Q_NULLPTR)
		: AbstractRecordEditor(ligh, dataFile), AbstractRecordDialog(parent) {
		ui.setupUi(this);

		ui.editEDID->setText(QString::fromStdString(ligh->EDID));

		ui.sbFOV->setValue(ligh->data.FOV);
		ui.sbFalloff->setValue(ligh->data.falloff);
		ui.sbRadius->setValue(ligh->data.radius);
		ui.sbNearClip->setValue(ligh->data.nearClip);
	};
};