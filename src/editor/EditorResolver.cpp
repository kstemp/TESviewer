#include "EditorResolver.h"

#include "REFReditor.h"
#include "STATeditor.h"
#include "LIGHeditor.h"

QWidget* createEditor(ESM::Record* record, ESM::RecordMap& recordMap) {
	switch (record->type) {
		//case ESM::RecordType::CELL:
			//return  new CELLeditor(static_cast<ESM::CELL*>(record), recordMap);
			// TODO we could do if (record = static_cast) then new editor(record) etc.

	case ESM::RecordType::STAT:
		return new STATeditor(static_cast<ESM::STAT*>(record), recordMap);

	case ESM::RecordType::REFR:
		return new REFReditor(static_cast<ESM::REFR*>(record), recordMap);

	case ESM::RecordType::LIGH:
		return new LIGHeditor(static_cast<ESM::LIGH*>(record), recordMap);

	default:
		return nullptr;
	}
}