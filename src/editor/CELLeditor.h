#pragma once

#include <algorithm>
#include <esm\records\CELL.h>
#include <esm\File.h>
#include "..\render\ModelViewer.h"
#include <qmainwindow.h>
#include <QToolbar>

class CELLeditor : public QMainWindow {
    Q_OBJECT

        ESM::CELL* cell;
    ESM::File& dataFile;

    ModelViewer* modelViewer = nullptr;

public:

    CELLeditor(ESM::CELL* cell, ESM::File& dataFile, QWidget* parent = Q_NULLPTR)
        : QMainWindow(parent), cell(cell), dataFile(dataFile) {

        modelViewer = new ModelViewer(this);

        setMinimumSize(1600, 1000);

        auto toolbar = new QToolBar();
        this->addToolBar(toolbar);
        toolbar->addAction("Inspector");
        toolbar->setMovable(false);

        modelViewer->setGeometry(0, 40, 1600, 1000);

        modelViewer->afterInitializedGL = [&]() {
            std::vector<ESM::Group>* cellChildrenTop = nullptr;

            cellChildrenTop = (cell->DATA & ESM::CellFlags::Interior) ?
                &dataFile.groups[57].subgroups[cell->getBlock()].subgroups[cell->getSubBlock()].subgroups
                : &dataFile.groups[58].subgroups[0].subgroups[1].subgroups[2].subgroups;

            const auto cellChildren = std::find_if(
                cellChildrenTop->begin(),
                cellChildrenTop->end(),
                [&](const auto& g) {
                    uint32_t groupParentFormID = *(uint32_t*)(&g.label[0]);
                    return g.type == ESM::GroupType::CellChildren && groupParentFormID == cell->formID;
                });

            const auto cellTemporaryChildren = std::find_if(
                cellChildren->subgroups.begin(),
                cellChildren->subgroups.end(),
                [](const auto& g) {
                    return g.type == ESM::GroupType::CellTemporaryChildren;
                });

            for (const auto& r : cellTemporaryChildren->records) {
                switch (r->type) {
                case ESM::RecordType::REFR:
                {
                    const auto refr = static_cast<ESM::REFR*>(r);

                    const auto correspondingRecord = dataFile.recordMap[refr->NAME];

                    if (correspondingRecord)
                        if (correspondingRecord->model())
                            modelViewer->addModel(correspondingRecord->model().value(), refr->DATA.position, refr->DATA.rotation);

                }
                break;
                }
            }
        };

    }


};