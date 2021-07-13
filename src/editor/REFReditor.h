#pragma once

#include <QDialog>
#include <esm\records\REFR.h>
#include "..\Config.h"
#include "..\util.h"
#include <ui_REFReditor.h>
#include <unordered_map>
#include <windows.h>

class REFReditor : public QDialog {
	Q_OBJECT

		ESM::REFR* refr;

	Ui::REFReditor ui;

	const std::unordered_map<uint32_t, ESM::Record*>& recordMap;

public:

	REFReditor(ESM::REFR* refr, const std::unordered_map<uint32_t, ESM::Record*>& recordMap, QWidget* parent = Q_NULLPTR)
		: QDialog(parent), refr(refr), recordMap(recordMap) {
		ui.setupUi(this);

		setWindowTitle(QString::fromStdString("Reference"));
		setWindowFlags(Qt::Drawer);
		setFixedSize(this->width(), this->height());

		const auto correspondingRecord = recordMap.find(refr->NAME);

		if (correspondingRecord != recordMap.end()) {
			ui.le_base->setText(QString::fromStdString(correspondingRecord->second->EDID + "[" + NumToHexStr(correspondingRecord->second->formID) + "]"));

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
	};

signals:
	void changed();
};