#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include <QMenu>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>

#include "optiondialog.h"

#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkProperty.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

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
    ui->treeView->expandAll();

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView,
            &QWidget::customContextMenuRequested,
            this,
            &MainWindow::onTreeViewContextMenuRequested);

    if (ui->toolBar) {
        ui->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }

    if (ui->actionOpenFile) {
        ui->actionOpenFile->setIcon(QIcon(":/Icons/Icons/fileopen.png"));
        ui->actionOpenFile->setIconVisibleInMenu(true);
    }

    if (ui->pushButton) {
        ui->pushButton->setText("Add Child");
    }

    if (ui->pushButton_2) {
        ui->pushButton_2->setText("Options");
    }

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

    ui->vtkWidget->setRenderWindow(renderWindow);
    renderWindow->AddRenderer(renderer);

    renderer->SetBackground(0.1, 0.2, 0.4);
    renderer->ResetCamera();

    ui->vtkWidget->renderWindow()->Render();

    emit statusUpdateMessage("Ready", 2000);
}

MainWindow::~MainWindow()
{
    delete ui;
    ui = nullptr;
}

void MainWindow::refreshRender()
{
    renderer->RemoveAllViewProps();
    addActorsFromTree(partList->getRootItem());
    renderer->ResetCamera();
    ui->vtkWidget->renderWindow()->Render();
}

void MainWindow::addActorsFromTree(ModelPart* item)
{
    if (!item) {
        return;
    }

    if (item->getActor() && item->isVisible()) {
        renderer->AddActor(item->getActor());
    }

    for (int i = 0; i < item->childCount(); ++i) {
        addActorsFromTree(item->child(i));
    }
}

void MainWindow::on_actionOpenFile_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open STL File",
        "",
        "STL Files (*.stl)"
        );

    if (filePath.isEmpty()) {
        emit statusUpdateMessage("Open File cancelled", 2000);
        return;
    }

    QFileInfo info(filePath);
    QString newName = info.fileName();

    QModelIndex currentIndex = ui->treeView->currentIndex();

    ModelPart* parentPart = nullptr;
    if (currentIndex.isValid()) {
        parentPart = static_cast<ModelPart*>(currentIndex.internalPointer());
    }

    if (!parentPart) {
        parentPart = partList->getRootItem();
    }

    auto* newPart = new ModelPart({newName, QString("true")}, parentPart);
    newPart->loadSTL(filePath);

    if (newPart->getActor()) {
        int siblingCount = parentPart->childCount();
        double xOffset = static_cast<double>(siblingCount) * 120.0;
        newPart->getActor()->SetPosition(xOffset, 0.0, 0.0);
    }

    parentPart->appendChild(newPart);

    ui->treeView->reset();
    ui->treeView->expandAll();

    refreshRender();

    emit statusUpdateMessage(
        QString("Loaded STL under selected item: %1").arg(newName),
        4000
        );
}

void MainWindow::on_pushButton_released()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();

    ModelPart* parentPart = nullptr;
    if (currentIndex.isValid()) {
        parentPart = static_cast<ModelPart*>(currentIndex.internalPointer());
    }

    if (!parentPart) {
        parentPart = partList->getRootItem();
    }

    int childNumber = parentPart->childCount();
    QString childName = QString("New Child %1").arg(childNumber);

    auto* newPart = new ModelPart({childName, QString("true")}, parentPart);
    parentPart->appendChild(newPart);

    ui->treeView->reset();
    ui->treeView->expandAll();

    emit statusUpdateMessage(
        QString("Added child item under: %1")
            .arg(parentPart->data(0).toString()),
        3000
        );
}

void MainWindow::on_pushButton_2_released()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("Select a tree item first", 3000);
        return;
    }

    auto* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart) {
        emit statusUpdateMessage("Invalid selection", 3000);
        return;
    }

    OptionDialog dialog(this);
    dialog.setFromModelPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.applyToModelPart(selectedPart);
        ui->treeView->reset();
        refreshRender();
        emit statusUpdateMessage("Options applied", 3000);
    }
}

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
        on_pushButton_2_released();
    }
}
