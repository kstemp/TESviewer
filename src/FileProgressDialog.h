#pragma once

#include <QDialog>

#include <ui_FileProgressDialog.h>

class FileProgressDialog : public QDialog {
	Q_OBJECT

		Ui::FileProgressDialog ui;
public slots:
	void setValue(const int val, const bool main = true) {
		if (main)
			ui.pbMain->setValue(val);
		else
			ui.pbFile->setValue(val);
	}

public:

	FileProgressDialog(QWidget* parent = Q_NULLPTR)
		: QDialog(parent) {
		ui.setupUi(this);
	}

	void setParsing(const QString& fileName) {
		ui.label->setText("Parsing " + fileName + "...");
	}
};