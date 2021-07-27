#include "EditorResolver.h"

#include "REFReditor.h"
#include "STATeditor.h"
#include "LIGHeditor.h"

QWidget* createEditor(ESM::Record* record, ESM::File& dataFile) {
	switch (record->type) {
		//case ESM::RecordType::CELL:
			//return  new CELLeditor(static_cast<ESM::CELL*>(record), recordMap);
			// TODO we could do if (record = static_cast) then new editor(record) etc.

	case ESM::RecordType::STAT:
		return new STATeditor(record->castTo<ESM::STAT>(), dataFile);

	case ESM::RecordType::REFR:
		return new REFReditor(record->castTo<ESM::REFR>(), dataFile);

	case ESM::RecordType::LIGH:
		return new LIGHeditor(record->castTo<ESM::LIGH>(), dataFile);

	default:
		return nullptr;
	}
}