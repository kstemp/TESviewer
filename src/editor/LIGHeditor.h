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

		setWindowTitle(QString::fromStdString("Light"));
		setWindowFlags(Qt::Drawer);
		setFixedSize(this->width(), this->height());
	};

signals:
	void changed();
};