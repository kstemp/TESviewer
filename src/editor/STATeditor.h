#pragma once

#include <esm\records\STAT.h>
#include "..\render\ModelViewer.h"

class STATeditor : public ModelViewer {
    Q_OBJECT
    const ESM::STAT* stat;

    void initializeGL() override {
        ModelViewer::initializeGL();

        if (!stat->MODL.empty())
            addModel(stat->MODL);

    }

public:

    STATeditor(const ESM::STAT* stat, QWidget* parent = Q_NULLPTR)
        : ModelViewer(parent), stat(stat) {

        setMinimumSize(600, 600);

    }
};