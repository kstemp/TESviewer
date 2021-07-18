#pragma once

#include <QDialog>
#include <esm\records\LIGH.h>
#include "..\Config.h"
#include "..\util.h"
#include <ui_LIGHeditor.h>
#include <unordered_map>
#include <windows.h>

class LIGHeditor : public QDialog {
	Q_OBJECT
		ESM::LIGH* ligh;

	Ui::LIGHeditor ui;

	const std::unordered_map<uint32_t, ESM::Record*>& recordMap;

public:

	LIGHeditor(ESM::LIGH* ligh, const std::unordered_map<uint32_t, ESM::Record*>& recordMap, QWidget* parent = Q_NULLPTR)
		: QDialog(parent), ligh(ligh), recordMap(recordMap) {
		ui.setupUi(this);

		setWindowTitle(QString::fromStdString("Light: " + ligh->EDID));
		setWindowFlags(Qt::Drawer);
		setFixedSize(this->width(), this->height());

		ui.editEDID->setText(QString::fromStdString(ligh->EDID));

		ui.sbFOV->setValue(ligh->data.FOV);
		ui.sbFalloff->setValue(ligh->data.falloff);
		ui.sbRadius->setValue(ligh->data.radius);
		ui.sbNearClip->setValue(ligh->data.nearClip);
	};

signals:
	void changed();
};