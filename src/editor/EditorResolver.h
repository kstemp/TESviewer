#pragma once

#include <esm\Record.h>
#include <esm\File.h>

#include <QWidget>

QWidget* createEditor(ESM::Record* record, ESM::RecordMap& recordMap);