#pragma once

#include <QAbstractTableModel>

#include <esm\Record.h>

class RecordModel : public QAbstractTableModel {
	Q_OBJECT

		ESM::Record* record;

public:
	RecordModel(ESM::Record* record, QObject* parent = nullptr) : record(record), QAbstractTableModel(parent) {}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		return record->fields.size();
	}

	int columnCount(const QModelIndex& parent = QModelIndex()) const override {
		// name and value!
		return 2;
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
		if (role == Qt::DisplayRole) {
			int column = index.column();
			int row = index.row();

			if (row < 0 || row >= record->fields.size())
				return QVariant();

			if (record->type == "GMST") {
				if (column == 0)
					return QString::fromStdString(record->fields[row].name);
				else if (column == 1) {
					if (row == 0)
						return QString::fromStdString((*record)["EDID"].string());
					else {
						switch ((*record)["EDID"].string()[0]) {
						case 'b':
							return (*record)["DATA"].get<bool>();
							break;

						case 'i':
							return (*record)["DATA"].get<uint32_t>();
							break;
						case 'f':
							return (*record)["DATA"].get<float>();
							break;

						case 's':
							return (*record)["DATA"].get<uint32_t>();
							break;

						default:
							return QVariant();
						}
					}
				}
				else
					return QVariant();
			}

			return QVariant();
		}

		return QVariant();
	}
};