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

		QString title = QString::fromStdString((*base)["EDID"].string() + "[" + NumToHexStr(base->formID) + "]");

		ui.le_base->setText(title);
		setWindowTitle("Reference to: " + title);

		ui.sb_posX->setValue(std::get<float>((*refr)["DATA"].struct_("position", "x")));
		ui.sb_posY->setValue(std::get<float>((*refr)["DATA"].struct_("position", "y")));
		ui.sb_posZ->setValue(std::get<float>((*refr)["DATA"].struct_("position", "z")));

		ui.sb_rotX->setValue(std::get<float>((*refr)["DATA"].struct_("rotation", "x")) * 57.2957795);
		ui.sb_rotY->setValue(std::get<float>((*refr)["DATA"].struct_("rotation", "y")) * 57.2957795);
		ui.sb_rotZ->setValue(std::get<float>((*refr)["DATA"].struct_("rotation", "z")) * 57.2957795);

		QWidget::connect(ui.sb_posX, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"].struct_("position", "x") = { i };
				emit changed();
			});

		QWidget::connect(ui.sb_posY, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"].struct_("position", "y") = { i };
				emit changed();
			});

		QWidget::connect(ui.sb_posZ, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"].struct_("position", "z") = { i };
				emit changed();
			});

		QWidget::connect(ui.sb_rotX, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"].struct_("rotation", "x") = { i / 57.2957795f };
				emit changed();
			});

		QWidget::connect(ui.sb_rotY, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"].struct_("rotation", "y") = { i / 57.2957795f };
				emit changed();
			});

		QWidget::connect(ui.sb_rotZ, &QSpinBox::valueChanged, this,
			[this](const int val) {
				float i = val;
				(*this->record)["DATA"].struct_("rotation", "z") = { i / 57.2957795f };
				emit changed();
			});

		QWidget::connect(ui.btnEditBase, &QToolButton::clicked, this, &REFReditor::editBase);
	};
};