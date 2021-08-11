#pragma once

#include "AbstractRecordEditor.h"
#include "AbstractRecordDialog.h"
#include "..\render\ModelViewer.h"
#include "..\Config.h"
#include <esm\File.h>
#include <esm\records\STAT.h>
#include <ui_STATeditor.h>

class MyMV : public ModelViewer {
	std::string m = "";

protected:
	void initializeGL() override {
		ModelViewer::initializeGL();

		addModel(m, Vector3(0, 0, 0), Vector3(0, 0, 0));
	}

public:

	MyMV(std::string model, QWidget* parent) : m(model), ModelViewer(parent) {}
};

class STATeditor : public AbstractRecordDialog, public AbstractRecordEditor {
	Q_OBJECT

		MyMV* modelViewer = nullptr;

	Ui::STATeditor ui;

public slots:

	void EDIDChanged(const QString& text) {
		if (!text.isEmpty()) {
			//	(*record)["EDID"].get < std::string>() = text.toStdString();
			//	record.modified = true;
		}
	}

public:

	STATeditor(ESM::Record* stat, ESM::File& dataFile, QWidget* parent = Q_NULLPTR)
		: AbstractRecordEditor(stat, dataFile), AbstractRecordDialog(parent) {
		ui.setupUi(this);

		//if (!record.MODL.empty()) {
			//	modelViewer = new MyMV(stat->MODL, stat->obnd, ui.gb_preview);
			//	modelViewer->setGeometry(19, 29, 341 + 2 * 19, 341 + 2 * 19);

		ui.edit_MODL->setText(QString::fromStdString(record->fieldOr<std::string>("MODL")));
		//}

		ui.sb_maxAngle->setValue((*record)["DNAM"]["maxAngle"].Float());

		ui.edit_EDID->setText(QString::fromStdString((*record)["EDID"].string()));

		ui.cb_treeLOD->setChecked(record->flags & ESM::STATFlags::hasTreeLOD);
		ui.cb_onLocalMap->setChecked(!(record->flags & ESM::STATFlags::notOnLocalMap));
		ui.cb_distantLOD->setChecked(record->flags & ESM::STATFlags::hasDistantLOD);
		//ui.cb_highDefLOD->setChecked(record.flags & ESM::STATFlags::highDefLOD);
		ui.cb_currents->setChecked(record->flags & ESM::STATFlags::currents);
		ui.cb_marker->setChecked(record->flags & ESM::STATFlags::marker);
		ui.cb_obstacle->setChecked(record->flags & ESM::STATFlags::obstacle);
		ui.cb_worldMap->setChecked(record->flags & ESM::STATFlags::worldMap);

		ui.cb_materialEDID->addItem("NONE");

		for (const auto& [formID, record] : dataFile.recordMap)
			if (record->type == "MATO")
				ui.cb_materialEDID->addItem(QString::fromStdString((*record)["EDID"].string()), formID);

		size_t index = ui.cb_materialEDID->findData((*record)["DNAM"]["formID"].uint32());

		ui.cb_materialEDID->setCurrentIndex(index != -1 ? index : 0);

		if (record->flags & ESM::STATFlags::navMesh_boundingBox)
			ui.rdb_boundingBox->setChecked(true);
		else if (record->flags & ESM::STATFlags::navMesh_filter)
			ui.rdb_filter->setChecked(true);
		else if (record->flags & ESM::STATFlags::navMesh_ground)
			ui.rdb_ground->setChecked(true);
		else
			ui.rdb_collision->setChecked(true);

		QWidget::connect(ui.edit_EDID, &QLineEdit::textChanged, this, &STATeditor::EDIDChanged);
	}
};