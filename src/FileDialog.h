#pragma once

#include <QDialog>
#include <QDirIterator>

#include "Config.h"
#include "util.h"
#include <set>
#include <qmessagebox.h>
#include <ui_FileDialog.h>

#include <esm\File.h>

class FileDialog : public QDialog {
	Q_OBJECT

		Ui::FileDialog ui;

	QSet<QString> filesToLoad = { };
	QString activeFile = "";

	QMap<QString, QList<QString>> masters;

	QTreeWidgetItem* getMasters(const QString& esm) {
		auto top = new QTreeWidgetItem((QTreeWidget*)nullptr, { esm });
		top->setCheckState(0, Qt::Unchecked);
		top->setDisabled(true);

		const auto& parents = masters[esm.endsWith('\0') ? esm.chopped(1) : esm];

		for (const auto& parent : parents)
			top->addChild(getMasters(parent));

		return top;
	};

	void check(QTreeWidgetItem* item, QString file) {
		QString text = item->text(0);

		if (text.endsWith('\0'))
			text.chop(1);

		QString filec = file;

		if (file.endsWith('\0'))
			filec = file.chopped(1);

		int len = text.length();
		int len1 = filec.length();

		if (text == filec)
			item->setCheckState(0, Qt::Checked);

		for (int i = 0; i < item->childCount(); ++i)
			check(item->child(i), file);
	}

private slots:
	void onItemChanged(QTreeWidgetItem* item, int column) {
		bool oldState = ui.treeFiles->blockSignals(true);

		if (item->checkState(0) == Qt::Checked) {
			QString file = item->text(0);

			for (size_t i = 0; i < ui.treeFiles->topLevelItemCount(); ++i) {
				QTreeWidgetItem* t = ui.treeFiles->topLevelItem(i);

				QString tx = t->text(0);

				filesToLoad.insert(file);

				check(t, file);
			}
		}
		else {
		}

		ui.treeFiles->blockSignals(oldState);

		updateToBeLoadedList();
	}

	void updateToBeLoadedList() {
		ui.lstLoaded->clear();

		for (const QString& s : filesToLoad)
			ui.lstLoaded->addItem(s);
	}

	void setActive() {
		auto selectedItems = ui.treeFiles->selectedItems();

		if (selectedItems.empty())
			return;

		auto item = selectedItems.front();
		filesToLoad.insert(item->text(0));

		if (item->checkState(0) == Qt::Unchecked)
			item->setCheckState(0, Qt::Checked);

		for (size_t i = 0; i < ui.treeFiles->topLevelItemCount(); ++i) {
			QTreeWidgetItem* t = ui.treeFiles->topLevelItem(i);
			t->setBackground(0, QBrush());
		}

		item->setBackground(0, QBrush(QColor::fromRgb(1, 255, 112)));

		activeFile = item->text(0);
	}

	void openFiles() {
		accept();
	}

	void cancel() {
		reject();
	}

public:

	const QString& getActiveFile() const {
		return activeFile;
	}

	const QSet<QString>& getFilesToLoad() const {
		return filesToLoad;
	}

	FileDialog(QWidget* parent = Q_NULLPTR)
		: QDialog(parent) {
		ui.setupUi(this);

		QWidget::connect(ui.treeFiles, &QTreeWidget::itemChanged, this, &FileDialog::onItemChanged);
		QWidget::connect(ui.btnSetActive, &QPushButton::clicked, this, &FileDialog::setActive);
		QWidget::connect(ui.btnOpen, &QPushButton::clicked, this, &FileDialog::openFiles);
		QWidget::connect(ui.btnCancel, &QPushButton::clicked, this, &FileDialog::cancel);

		QDirIterator it(QString::fromStdString(Config::SKYRIM_DATA_DIR), { "*.esm", "*.esp" });

		while (it.hasNext()) {
			QFile file(it.next());
			QFileInfo fileInfo(file);

			QString fileName = fileInfo.fileName();

			ESM::File dataFile(fileName);

			try {
				dataFile.parse(file.fileName().toStdString(), true);

				auto& m = masters[fileName];

				for (const std::string s : dataFile.header->MAST)
					m.push_back(QString::fromStdString(s + "\0"));
			}
			catch (const std::runtime_error& e) {
				// TODO cleanup the header pointer
				//dataFile.recordMap.
				QMessageBox msgbox;
				msgbox.critical(0, "Error parsing file header", QString::fromStdString("Error parsing " + fileName.toStdString() + ": " + e.what()));
			}
		}

		for (auto it = masters.begin(); it != masters.end(); ++it) {
			QString fileName = it.key();
			QStringList masterList = it.value();

			auto top = new QTreeWidgetItem((QTreeWidget*)nullptr, { fileName });
			top->setCheckState(0, Qt::Unchecked);

			for (QString master : masterList)
				top->addChild(getMasters(master));

			ui.treeFiles->addTopLevelItem(top);
		}
	};
};