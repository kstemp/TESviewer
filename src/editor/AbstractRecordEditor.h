#pragma once

#include <esm\File.h>

template<typename T>
class AbstractRecordEditor {
protected:
	T* record;

	ESM::File& dataFile;

public:

	AbstractRecordEditor(T* record, ESM::File& dataFile)
		:record(record), dataFile(dataFile) {
	};
};