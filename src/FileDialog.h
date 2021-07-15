#pragma once

#include <QDialog>
#include <QDirIterator>

#include "Config.h"
#include "util.h"
#include <ui_FileDialog.h>

#include <esm\File.h>

class FileDialog : public QDialog {
	Q_OBJECT

		Ui::FileDialog ui;

public:

	FileDialog(QWidget* parent = Q_NULLPTR)
		: QDialog(parent) {
		ui.setupUi(this);

		ui.groupBox->hide();

		QDirIterator it(QString::fromStdString(SKYRIM_DATA_DIR), { "*.esm", "*.esp" });

		while (it.hasNext()) {
			QFile file(it.next());
			QFileInfo fileInfo(file);

			ESM::File dataFile;
			dataFile.parse(file.fileName().toStdString(), true);

			// list parent masters underneath the items
			auto top = new QTreeWidgetItem((QTreeWidget*)nullptr, { fileInfo.fileName() });

			for (const std::string& s : dataFile.header->MAST) {
				auto parentMaster = new QTreeWidgetItem((QTreeWidget*)nullptr, { QString::fromStdString(s) });
				parentMaster->setCheckState(0, Qt::Unchecked);
				parentMaster->setDisabled(true);
				top->addChild(parentMaster);
			}

			top->setCheckState(0, Qt::Unchecked);

			ui.treeFiles->addTopLevelItem(top);
		}
	};
};