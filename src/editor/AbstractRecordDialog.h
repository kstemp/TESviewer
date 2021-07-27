#pragma once

#include <QDialog>

class AbstractRecordDialog : public QDialog {
	Q_OBJECT

public:

	AbstractRecordDialog(QWidget* parent = nullptr) : QDialog(parent) {}

signals:
	void changed();
};