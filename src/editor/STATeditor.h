#pragma once

#include <QDialog>
#include <esm\records\MATO.h>
#include <esm\records\STAT.h>
#include <esm\Record.h>
#include "..\render\ModelViewer.h"
#include "..\Config.h"
#include <ui_STATeditor.h>
#include <unordered_map>

class MyMV : public ModelViewer {
	std::string m = "";

protected:
	void initializeGL() override {
		ModelViewer::initializeGL();

		addModel(m);
	}

public:

	MyMV(std::string model, QWidget* parent) : m(model), ModelViewer(parent) {}
};

class STATeditor : public QDialog {
	Q_OBJECT
		const ESM::STAT* stat;

	MyMV* modelViewer = nullptr;

	Ui::STATeditor ui;

	const std::unordered_map<uint32_t, ESM::Record*>& recordMap;

public:

	STATeditor(const ESM::STAT* stat, const std::unordered_map<uint32_t, ESM::Record*>& recordMap, QWidget* parent = Q_NULLPTR)
		: QDialog(parent), stat(stat), recordMap(recordMap) {
		ui.setupUi(this);

		setWindowTitle(QString::fromStdString("Static: " + stat->EDID));
		setWindowFlags(Qt::Drawer);
		setFixedSize(this->width(), this->height());

		if (!stat->MODL.empty()) {
			modelViewer = new MyMV(stat->MODL, ui.gb_preview);
			modelViewer->setGeometry(19, 29, 341 + 2 * 19, 341 + 2 * 19);

			ui.edit_MODL->setText(QString::fromStdString(stat->MODL));
		}

		ui.sb_maxAngle->setValue(stat->maxAngle);

		ui.edit_EDID->setText(QString::fromStdString(stat->EDID));

		ui.cb_treeLOD->setChecked(stat->flags & ESM::STATFlags::hasTreeLOD);
		ui.cb_onLocalMap->setChecked(!(stat->flags & ESM::STATFlags::notOnLocalMap));
		ui.cb_distantLOD->setChecked(stat->flags & ESM::STATFlags::hasDistantLOD);
		ui.cb_highDefLOD->setChecked(stat->flags & ESM::STATFlags::highDefLOD);
		ui.cb_currents->setChecked(stat->flags & ESM::STATFlags::currents);
		ui.cb_marker->setChecked(stat->flags & ESM::STATFlags::marker);
		ui.cb_obstacle->setChecked(stat->flags & ESM::STATFlags::obstacle);
		ui.cb_worldMap->setChecked(stat->flags & ESM::STATFlags::worldMap);

		ui.cb_materialEDID->addItem("NONE");

		for (const auto& [formID, record] : recordMap)
			if (record->type == ESM::RecordType::MATO)
				ui.cb_materialEDID->addItem(QString::fromStdString(record->EDID), formID);

		size_t index = ui.cb_materialEDID->findData(stat->MATOformID);

		ui.cb_materialEDID->setCurrentIndex(index != -1 ? index : 0);

		if (stat->flags & ESM::STATFlags::navMesh_boundingBox)
			ui.rdb_boundingBox->setChecked(true);
		else if (stat->flags & ESM::STATFlags::navMesh_filter)
			ui.rdb_filter->setChecked(true);
		else if (stat->flags & ESM::STATFlags::navMesh_ground)
			ui.rdb_ground->setChecked(true);
		else
			ui.rdb_collision->setChecked(true);
	}
};