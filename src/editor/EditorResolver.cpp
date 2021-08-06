#include "EditorResolver.h"

#include "REFReditor.h"
#include "STATeditor.h"
#include "LIGHeditor.h"

#include "RecordEditor.h"

QWidget* createEditor(ESM::Record* record, ESM::File& dataFile) {
	/*if (record->type == "STAT")
		return new STATeditor(record, dataFile);
	else 	if (record->type == "REFR")
		return new REFReditor(record, dataFile);
	else	if (record->type == "LIGH")
		return new LIGHeditor(record, dataFile);
	else*/
	return new RecordEditor(record, dataFile);
}