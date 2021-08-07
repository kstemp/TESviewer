#pragma once

#include <QAbstractTableModel>

#include <esm\Group.h>
#include <esm\util.h>

class CellChildrenModel : public QAbstractTableModel {
	Q_OBJECT

		ESM::Group* cellTempChildren;
	ESM::File& dataFile;

public:
	CellChildrenModel(ESM::Group* cellTempChildren, ESM::File& dataFile, QObject* parent = nullptr) : dataFile(dataFile), cellTempChildren(cellTempChildren), QAbstractTableModel(parent) {}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		if (cellTempChildren)
			return cellTempChildren->records.size();

		return 0;
	}

	int columnCount(const QModelIndex& parent = QModelIndex()) const override {
		// EDID, formId in hex and full name of base object type
		return 3;
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
		if (role == Qt::DisplayRole) {
			int column = index.column();
			int row = index.row();

			if (column < 0 || column > 2)
				return QVariant();

			if (row < 0 || row >= cellTempChildren->records.size())
				return QVariant();

			ESM::Record* r = cellTempChildren->records[row];

			if (r->type == "REFR") {
				ESM::Record* base = ESM::getBaseFromREFR(r, dataFile);

				if (base) {
					switch (column) {
					case 0:
						return QString::fromStdString((*base)["EDID"].string());
						break;
					case 1:
						return QString::fromStdString(NumToHexStr(r->formID));
						break;
					case 2:
						return QString::fromStdString(ESM::getRecordFullName(base->type));
						break;
					}
				}

				return QVariant();
			}
			else {
				switch (column) {
				case 0:
				{
					for (auto f : r->fields)
						if (f.name == "EDID")
							return QString::fromStdString(f.string());

					return "[no EDID]"
				}

				break;
				case 1:
					return QString::fromStdString(NumToHexStr(r->formID));
					break;
				case 2:
					return QString::fromStdString(ESM::getRecordFullName(r->type));
					break;
				}
			}
		}

		return QVariant();
	}
};