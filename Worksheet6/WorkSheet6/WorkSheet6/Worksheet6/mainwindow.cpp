#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create/allocate the model
    this->partList = new ModelPartList("Parts List", this);

    // Link model to the treeview
    ui->treeView->setModel(this->partList);

    // Build a demo multi-level tree (3 top-level, each with 5 children)
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

    // Optional: adjust column width
    ui->treeView->setColumnWidth(0, 180);
}

MainWindow::~MainWindow()
{
    delete ui;
    ui = nullptr;
}
