#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include <QPushButton>
#include <QMessageBox>
#include <QMenu>

#include "optiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Status bar signal -> status bar slot
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    // Button 1 -> messagebox + statusbar
    connect(ui->pushButton, &QPushButton::released,
            this, &MainWindow::handleButton);

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

    // ✅ Signoff 4: enable custom context menu on treeview
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QWidget::customContextMenuRequested,
            this, &MainWindow::onTreeViewContextMenuRequested);
}

MainWindow::~MainWindow()
{
    delete ui;
    ui = nullptr;
}

void MainWindow::handleButton()
{
    QMessageBox msgBox;
    msgBox.setText("Button 1 was clicked");
    msgBox.exec();

    emit statusUpdateMessage("Button 1 clicked", 3000);
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
    // Keep button 2 working (optional)
    openOptionsDialogForIndex(ui->treeView->currentIndex());
}

void MainWindow::onTreeViewContextMenuRequested(const QPoint& pos)
{
    // pos is in viewport coordinates
    QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid()) {
        emit statusUpdateMessage("Right-clicked empty area", 2000);
        return;
    }

    QMenu menu(this);
    QAction* actOptions = menu.addAction("Options...");

    // Show at mouse position (global)
    QAction* chosen = menu.exec(ui->treeView->viewport()->mapToGlobal(pos));
    if (chosen == actOptions) {
        // Make sure the clicked item becomes current
        ui->treeView->setCurrentIndex(index);
        openOptionsDialogForIndex(index);
    }
}
