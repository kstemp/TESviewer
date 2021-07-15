#pragma once

#include <unordered_map>

#include <QDialog>
#include <ui_REFReditor.h>

#include "..\util.h"
#include "..\Config.h"

#include "EditorResolver.h"
#include <esm\records\REFR.h>

class REFReditor : public QDialog {
	Q_OBJECT

		ESM::REFR* refr;

	Ui::REFReditor ui;

	ESM::RecordMap& recordMap;

public slots:

	void editBase() {
		// TODO avoid duplicates!
		const auto it = recordMap.find(refr->NAME);
		if (it != recordMap.end()) {
			ESM::Record* base = it->second;

			auto editor = createEditor(base, recordMap);

			if (editor)
				editor->show();
		}
	}

public:

	REFReditor(ESM::REFR* refr, ESM::RecordMap& recordMap, QWidget* parent = Q_NULLPTR)
		: QDialog(parent), refr(refr), recordMap(recordMap) {
		ui.setupUi(this);

		auto it = recordMap.find(refr->NAME);
		if (it != recordMap.end()) {
			ESM::Record* base = it->second;

			QString title = QString::fromStdString(base->EDID + "[" + NumToHexStr(base->formID) + "]");

			ui.le_base->setText(title);
			setWindowTitle("Reference to: " + title);

			ui.sb_posX->setValue(refr->DATA.position.x);
			ui.sb_posY->setValue(refr->DATA.position.y);
			ui.sb_posZ->setValue(refr->DATA.position.z);

			ui.sb_rotX->setValue(refr->DATA.rotation.x * 57.2957795);
			ui.sb_rotY->setValue(refr->DATA.rotation.y * 57.2957795);
			ui.sb_rotZ->setValue(refr->DATA.rotation.z * 57.2957795);
		}

		QWidget::connect(ui.sb_posX, &QSpinBox::valueChanged, this,
			[this](const int val) {
				this->refr->DATA.position.x = val;
				emit changed();
			});

		QWidget::connect(ui.sb_posY, &QSpinBox::valueChanged, this,
			[this](const int val) {
				this->refr->DATA.position.y = val;
				emit changed();
			});

		QWidget::connect(ui.sb_posZ, &QSpinBox::valueChanged, this,
			[this](const int val) {
				this->refr->DATA.position.z = val;
				emit changed();
			});

		QWidget::connect(ui.sb_rotX, &QSpinBox::valueChanged, this,
			[this](const int val) {
				this->refr->DATA.rotation.x = val / 57.2957795;
				emit changed();
			});

		QWidget::connect(ui.sb_rotY, &QSpinBox::valueChanged, this,
			[this](const int val) {
				this->refr->DATA.rotation.y = val / 57.2957795;
				emit changed();
			});

		QWidget::connect(ui.sb_rotZ, &QSpinBox::valueChanged, this,
			[this](const int val) {
				this->refr->DATA.rotation.z = val / 57.2957795;
				emit changed();
			});

		QWidget::connect(ui.btnEditBase, &QToolButton::clicked, this, &REFReditor::editBase);
	};

signals:
	void changed();
};