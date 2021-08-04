#pragma once

#include <esm\Record.h>
#include <esm\File.h>

class AbstractRecordEditor {
protected:
	ESM::Record* record;

	ESM::File& dataFile;

public:

	AbstractRecordEditor(ESM::Record* record, ESM::File& dataFile)
		:record(record), dataFile(dataFile) {
	};
};