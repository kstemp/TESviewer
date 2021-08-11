#pragma once

#include <QDialog>
#include "AbstractRecordEditor.h"
#include "AbstractRecordDialog.h"
#include <ui_REFReditor.h>

#include "..\util.h"
#include "..\Config.h"

#include "EditorResolver.h"
#include <esm\Util.h>

class REFReditor : public AbstractRecordDialog, public AbstractRecordEditor {
	Q_OBJECT

		Ui::REFReditor ui;

	ESM::Record* base;

public slots:

	void editBase() {
		auto editor = createEditor(base, dataFile);

		if (editor)
			editor->show();
	}

public:

	REFReditor(ESM::Record* refr, ESM::File& dataFile, QWidget* parent = Q_NULLPTR)
		: AbstractRecordEditor(refr, dataFile), AbstractRecordDialog(parent) {
		ui.setupUi(this);

		base = ESM::getBaseFromREFR(refr, dataFile);

		QString title = QString::fromStdString(base->fieldOr<std::string>("EDID", "")) + "[" + NumToHexStr(base->formID) + "]";

		ui.le_base->setText(title);
		setWindowTitle("Reference to: " + title);

		ui.sb_posX->setValue((*refr)["DATA"]("position", "x").Float());
		ui.sb_posY->setValue((*refr)["DATA"]("position", "y").Float());
		ui.sb_posZ->setValue((*refr)["DATA"]("position", "z").Float());

		ui.sb_rotX->setValue((*refr)["DATA"]("rotation", "x").Float() * 57.2957795);
		ui.sb_rotY->setValue((*refr)["DATA"]("rotation", "y").Float() * 57.2957795);
		ui.sb_rotZ->setValue((*refr)["DATA"]("rotation", "z").Float() * 57.2957795);

		QWidget::connect(ui.sb_posX, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"]("position", "x") = { i };
				emit changed();
			});

		QWidget::connect(ui.sb_posY, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"]("position", "y") = { i };
				emit changed();
			});

		QWidget::connect(ui.sb_posZ, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"]("position", "z") = { i };
				emit changed();
			});

		QWidget::connect(ui.sb_rotX, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"]("rotation", "x") = { i / 57.2957795f };
				emit changed();
			});

		QWidget::connect(ui.sb_rotY, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"]("rotation", "y") = { i / 57.2957795f };
				emit changed();
			});

		QWidget::connHeader.hect(ui.sb_rotZ, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"]("rotation", "z") = { i / 57.2957795f };
				emit changed();
			});

		QWidget::connect(ui.btnEditBase, &QToolButton::clicked, this, &REFReditor::editBase);
	};
};