#pragma once

#include <QtWidgets/QMainWindow>
#include <qmenubar.h>
#include <QAction>
#include <qpushbutton.h>
#include <qtreewidget.h>

#include <esm\File.h>

class CK : public QMainWindow {
	Q_OBJECT

public:
	CK(QWidget* parent = Q_NULLPTR);

private:

	void populateRecordList();

	ESM::File dataFile;

	QMenu* fileMenu;
	QAction* fileOpenAction;

	QTreeWidget* treeRecords;

private slots:

	void fileOpen();

	void onTreeViewItemClicked(QTreeWidgetItem* item, int column);
};
