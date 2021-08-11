#pragma once

#include <qabstractitemmodel.h>

#include <esm\Group.h>
#include <esm\util.h>

class RecordTreeModel : public QAbstractItemModel {
	Q_OBJECT

		QList<ESM::File>& dataFiles;

public:
	RecordTreeModel(QList<ESM::File>& dataFiles, QObject* parent = nullptr) : dataFiles(dataFiles), QAbstractItemModel(parent) {}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		return dataFiles.size();
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

			if (row < 0 || row >= dataFiles.size())
				return QVariant();

			return dataFiles[row].fileName;
		}

		return QVariant();
	}

	QModelIndex index(int row, int column, const QModelIndex& parent) const override {
		//if (!hasIndex(row, column, parent))
		//return QModelIndex();
		return createIndex(row, column);
		/*TreeItem* parentItem;

		if (!parent.isValid())
			parentItem = rootItem;
		else
			parentItem = static_cast<TreeItem*>(parent.internalPointer());

		TreeItem* childItem = parentItem->child(row);
		if (childItem)
			return createIndex(row, column, childItem);
		return QModelIndex();(*/
	}

	QModelIndex parent(const QModelIndex& index) const {
		//if (!index.isValid())
		return QModelIndex();

		/*TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
		TreeItem* parentItem = childItem->parentItem();

		if (parentItem == rootItem)
			return QModelIndex();

		return createIndex(parentItem->row(), 0, parentItem);*/
	}
};