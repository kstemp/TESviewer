#include "CK.h"

#include <qfiledialog.h>
#include <qboxlayout.h>
#include <chrono>

#include "Config.h"

#include  "util.h"

#include <esm\records\CELL.h>
#include "editor\CELLeditor.h"
#include "editor\STATeditor.h"

CK::CK(QWidget* parent)
    : QMainWindow(parent) {

    setWindowTitle(tr("ESMedit"));
    setMinimumSize(600, 800);

    // create actions
    fileOpenAction = new QAction(tr("Open master file..."), this);
    fileOpenAction->setShortcuts(QKeySequence::Open);
    QWidget::connect(fileOpenAction, &QAction::triggered, this, &CK::fileOpen);

    // create menus
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fileOpenAction);

    treeRecords = new QTreeWidget();
    treeRecords->setColumnCount(2);
    treeRecords->setHeaderLabels({ "Form ID", "Editor ID" });
    treeRecords->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeRecords->setExpandsOnDoubleClick(true);
    treeRecords->setAlternatingRowColors(true);

    connect(treeRecords, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onTreeViewItemClicked(QTreeWidgetItem*, int)));

    setCentralWidget(treeRecords);

}

void CK::fileOpen() {

    QString fileName = QFileDialog::getOpenFileName(
        this, 
        tr("Load Master File"), 
        QString::fromStdString(SKYRIM_DATA_DIR), 
        tr("Bethesda Master File (*.esm)")
    );

    auto a = std::chrono::steady_clock::now();
    dataFile.parse(fileName.toStdString());
    auto b = std::chrono::steady_clock::now();

    setWindowTitle(QString::number((std::chrono::duration_cast<std::chrono::microseconds>(b - a).count()) / 1000000.0));

    populateRecordList();

}

QTreeWidgetItem* getItemFromRecord(const ESM::Record* record){

    auto item = new QTreeWidgetItem((QTreeWidget*)nullptr/*, {  }*/);

    item->setText(0, QString::fromStdString(NumToHexStr(record->formID)));
    item->setText(1, QString::fromStdString(record->EDID));
    item->setData(0, Qt::UserRole, record->formID);
    item->setData(1, Qt::UserRole, record->formID);

    return item;

};

QTreeWidgetItem* loopChildGroups(const ESM::Group& group, const QString& title = "") {

    QString itemTitle = title.isEmpty() ? "sub " + QString::number(group.type) : title;
    auto item = new QTreeWidgetItem((QTreeWidget*)nullptr, { itemTitle });

    // all top-level records
    for (const auto& record : group.records)
        item->addChild(getItemFromRecord(record));

    for (const auto& subgroup : group.subgroups)
        item->addChild(loopChildGroups(subgroup));

    return item;

};

void CK::populateRecordList() {

    treeRecords->clear();

    for (const auto& group : dataFile.groups)
        treeRecords->addTopLevelItem(loopChildGroups(group, group.label));

}

void CK::onTreeViewItemClicked(QTreeWidgetItem* item, int column) {

    if (!item->data(0, Qt::UserRole).isValid())
        return;

    uint32_t formID = item->data(0, Qt::UserRole).toInt();

    auto record = dataFile.recordMap[formID]; // TODO replace with find() or sth, because the [] operator will create object for us
    if (!record) // TODO should not happen
        return;

    QWidget* editor = Q_NULLPTR;

    switch (record->type) {
    case ESM::RecordType::CELL:
        editor = new CELLeditor(static_cast<ESM::CELL*>(record), dataFile);
    break;

    case ESM::RecordType::STAT:
        editor = new STATeditor(static_cast<ESM::STAT*>(record));
    break;
    }

    if (editor)
        editor->show();

}