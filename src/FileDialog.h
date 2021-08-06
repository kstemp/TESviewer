#pragma once

#include <QDialog>

#include <set>

#include <ui_FileDialog.h>

#include <esm\File.h>

class FileDialog : public QDialog {
	Q_OBJECT

		Ui::FileDialog ui;

	QSet<QString> filesToLoad = { };
	QString activeFile = "";

	QMap<QString, QList<QString>> masters;

	QTreeWidgetItem* getMasters(const QString& esm);

	void check(QTreeWidgetItem* item, QString file);

private slots:
	void onItemChanged(QTreeWidgetItem* item, int column);

	void updateToBeLoadedList();

	void setActive();

	void openFiles();

	void cancel();

public:

	const QString& getActiveFile() const;

	const QSet<QString>& getFilesToLoad() const;

	FileDialog(QWidget* parent = Q_NULLPTR);
};