#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include <QMenu>
#include <QFileDialog>
#include <QFileInfo>

#include "optiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Status bar signal -> status bar slot
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    // Tree model
    this->partList = new ModelPartList("Parts List", this);
    ui->treeView->setModel(this->partList);

    // Demo tree
    ModelPart* rootItem = this->partList->getRootItem();
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");
        auto* childItem = new ModelPart({name, visible}, rootItem);
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString childName = QString("Item %1,%2").arg(i).arg(j);
            QString childVisible("true");
            auto* childChild = new ModelPart({childName, childVisible}, childItem);
            childItem->appendChild(childChild);
        }
    }
    ui->treeView->setColumnWidth(0, 180);

    // Signoff 4: enable custom context menu on treeview
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QWidget::customContextMenuRequested,
            this, &MainWindow::onTreeViewContextMenuRequested);
}

MainWindow::~MainWindow()
{
    delete ui;
    ui = nullptr;
}

void MainWindow::openOptionsDialogForIndex(const QModelIndex& index)
{
    if (!index.isValid()) {
        emit statusUpdateMessage("No tree item selected", 3000);
        return;
    }

    auto* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) {
        emit statusUpdateMessage("Invalid selection", 3000);
        return;
    }

    OptionDialog dialog(this);
    dialog.setFromModelPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.applyToModelPart(selectedPart);

        // Update view via model so it emits dataChanged
        ui->treeView->model()->setData(index.siblingAtColumn(0), selectedPart->data(0));
        ui->treeView->model()->setData(index.siblingAtColumn(1), selectedPart->data(1));

        emit statusUpdateMessage("Dialog accepted: item updated", 3000);
    } else {
        emit statusUpdateMessage("Dialog rejected: no changes", 3000);
    }
}

void MainWindow::on_pushButton_2_released()
{
    // Options button
    openOptionsDialogForIndex(ui->treeView->currentIndex());
}

void MainWindow::onTreeViewContextMenuRequested(const QPoint& pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid()) {
        emit statusUpdateMessage("Right-clicked empty area", 2000);
        return;
    }

    QMenu menu(this);
    QAction* actOptions = menu.addAction("Options...");

    QAction* chosen = menu.exec(ui->treeView->viewport()->mapToGlobal(pos));
    if (chosen == actOptions) {
        ui->treeView->setCurrentIndex(index);
        openOptionsDialogForIndex(index);
    }
}

void MainWindow::on_pushButton_released()
{
    // ✅ Signoff 5: Open File Dialog -> use selected file to change item name
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("Select a tree item first", 3000);
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open File",
        "",
        "All Files (*.*)"
        );

    if (filePath.isEmpty()) {
        emit statusUpdateMessage("Open File cancelled", 2000);
        return;
    }

    QFileInfo info(filePath);
    QString newName = info.baseName(); // no extension (clearer for demo)

    auto* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) {
        emit statusUpdateMessage("Invalid selection", 3000);
        return;
    }

    // Update model data
    selectedPart->setData(0, newName);
    ui->treeView->model()->setData(index.siblingAtColumn(0), newName);

    emit statusUpdateMessage(QString("Loaded: %1").arg(info.fileName()), 4000);
}
#include <QFileDialog>
#include <QFileInfo>

void MainWindow::on_actionOpenFile_triggered()
{
    // 1) 必须先选中 tree item
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("Select a tree item first", 3000);
        return;
    }

    // 2) 打开文件选择框
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open File",
        "",
        "All Files (*.*)"
        );

    if (filePath.isEmpty()) {
        emit statusUpdateMessage("Open File cancelled", 2000);
        return;
    }

    // 3) 用文件名改 item 的名字
    QFileInfo info(filePath);
    QString newName = info.fileName();   // 带后缀，老师一眼看得出来自文件

    auto* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) {
        emit statusUpdateMessage("Invalid selection", 3000);
        return;
    }

    selectedPart->setData(0, newName);
    ui->treeView->model()->setData(index.siblingAtColumn(0), newName);

    // 4) status bar 提示
    emit statusUpdateMessage(QString("Loaded: %1").arg(newName), 4000);
}
