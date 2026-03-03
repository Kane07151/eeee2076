#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include "optiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect our status message signal to the status bar
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    // Create model + link to treeview
    this->partList = new ModelPartList("Parts List", this);
    ui->treeView->setModel(this->partList);

    // Demo tree (3 top-level, each 5 children)
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
}

MainWindow::~MainWindow()
{
    delete ui;
    ui = nullptr;
}

void MainWindow::on_pushButton_2_released()
{
    QModelIndex index = ui->treeView->currentIndex();
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

        // Update the view by writing via the model (so it emits dataChanged)
        ui->treeView->model()->setData(index.siblingAtColumn(0), selectedPart->data(0));
        ui->treeView->model()->setData(index.siblingAtColumn(1), selectedPart->data(1));

        emit statusUpdateMessage("Dialog accepted: item updated", 3000);
    } else {
        emit statusUpdateMessage("Dialog rejected: no changes", 3000);
    }
}
