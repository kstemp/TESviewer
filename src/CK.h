#pragma once

#include <QtWidgets/QMainWindow>
#include <qmenubar.h>
#include <QAction>
#include <qpushbutton.h>
#include <qtreewidget.h>

#include <ui_MainWindow.h>

#include <esm\File.h>

class CK : public QMainWindow {
	Q_OBJECT

public:
	CK(QWidget* parent = Q_NULLPTR);

private:

	Ui::MainWindow ui;

	void populateRecordList();
	QTreeWidgetItem* getItemFromRecord(const ESM::Record* record);
	QTreeWidgetItem* loopChildGroups(const ESM::Group& group, const QString& title = "");

	ESM::File dataFile;

private slots:

	void fileOpen();

	void onTreeViewItemClicked(QTreeWidgetItem* item, int column);
};
