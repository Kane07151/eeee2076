#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include <QMenu>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>

#include "optiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // =========================
    // 1. Status bar connection
    // =========================
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    // =========================
    // 2. Model + TreeView
    // =========================
    partList = new ModelPartList("Parts List", this);
    ui->treeView->setModel(partList);

    ModelPart* rootItem = partList->getRootItem();
    for (int i = 0; i < 3; i++) {
        auto* top = new ModelPart(
            {QString("TopLevel %1").arg(i), QString("true")},
            rootItem
            );
        rootItem->appendChild(top);

        for (int j = 0; j < 5; j++) {
            top->appendChild(
                new ModelPart(
                    {QString("Item %1,%2").arg(i).arg(j), QString("true")},
                    top
                    )
                );
        }
    }

    ui->treeView->setColumnWidth(0, 180);

    // =========================
    // 3. Context menu
    // =========================
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView,
            &QWidget::customContextMenuRequested,
            this,
            &MainWindow::onTreeViewContextMenuRequested);

    // =========================
    // 4. 强制修复工具栏图标
    // =========================
    if (ui->toolBar) {
        ui->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }

    if (ui->actionOpenFile) {
        ui->actionOpenFile->setIcon(
            QIcon(":/Icons/Icons/fileopen.png")
            );
        ui->actionOpenFile->setIconVisibleInMenu(true);
    }

    emit statusUpdateMessage("Ready", 2000);
}

MainWindow::~MainWindow()
{
    delete ui;
    ui = nullptr;
}

// ======================================================
// Open File 功能（老师第 5 部分核心）
// ======================================================

void MainWindow::on_actionOpenFile_triggered()
{
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
    QString newName = info.fileName();

    auto* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart) {
        emit statusUpdateMessage("Invalid selection", 3000);
        return;
    }

    selectedPart->setData(0, newName);
    ui->treeView->model()->setData(
        index.siblingAtColumn(0),
        newName
        );

    emit statusUpdateMessage(
        QString("Loaded: %1").arg(newName),
        4000
        );
}

// ======================================================
// 左按钮
// ======================================================

void MainWindow::on_pushButton_released()
{
    emit statusUpdateMessage("Left button clicked", 2000);

    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid())
        return;

    auto* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart)
        return;

    OptionDialog dialog(this);
    dialog.setFromModelPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.applyToModelPart(selectedPart);
        emit statusUpdateMessage("Options applied", 3000);
    }
}

// ======================================================
// 右按钮
// ======================================================

void MainWindow::on_pushButton_2_released()
{
    emit statusUpdateMessage("Right button clicked", 2000);
}

// ======================================================
// 右键菜单
// ======================================================

void MainWindow::onTreeViewContextMenuRequested(const QPoint& pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid())
        return;

    QMenu menu(this);
    QAction* actOptions = menu.addAction("Options...");

    QAction* chosen =
        menu.exec(ui->treeView->viewport()->mapToGlobal(pos));

    if (chosen == actOptions) {
        ui->treeView->setCurrentIndex(index);
        on_pushButton_released();
    }
}
