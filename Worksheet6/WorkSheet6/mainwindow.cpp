#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "ModelPart.h"
#include "optiondialog.h"

#include <QMessageBox>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QFileDialog>
#include <QFileInfo>
#include <QCursor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Buttons
    ui->pushButton->setText("Load");
    ui->pushButton_2->setText("Exit");

    // Status bar
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    // Buttons -> slots
    connect(ui->pushButton, &QPushButton::released,
            this, &MainWindow::handleLeftButton);
    connect(ui->pushButton_2, &QPushButton::released,
            this, &MainWindow::handleRightButton);

    // Tree model
    partList = new ModelPartList("Parts List");
    ui->treeView->setModel(partList);

    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::onTreeClicked);

    buildDemoTree();
    ui->treeView->expandAll();
    ui->treeView->resizeColumnToContents(0);

    emit statusUpdateMessage("Tree model loaded.", 2000);

    // Menu/Toolbar Open File action
    actionOpen_File = new QAction(QIcon(":/icons/open_file.png"), "Open File", this);
    connect(actionOpen_File, &QAction::triggered,
            this, &MainWindow::handleOpenFileAction);

    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(actionOpen_File);

    QToolBar* toolBar = addToolBar("Main");
    toolBar->addAction(actionOpen_File);

    // Context menu (Designer action: actionItem_Options)
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeView->addAction(ui->actionItem_Options);

    // 为了 100% 保证触发，不靠自动连接：我们手动 connect 一次
    connect(ui->actionItem_Options, &QAction::triggered,
            this, &MainWindow::on_actionItem_Options_triggered);
}

MainWindow::~MainWindow()
{
    delete partList;
    delete ui;
}

void MainWindow::buildDemoTree()
{
    ModelPart* rootItem = partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible });
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString subName = QString("Item %1,%2").arg(i).arg(j);
            QString subVisible("true");

            ModelPart* childChildItem = new ModelPart({ subName, subVisible });
            childItem->appendChild(childChildItem);
        }
    }
}

void MainWindow::handleLeftButton()
{
    QMessageBox::information(this, "Load", "Load clicked!");
    emit statusUpdateMessage("Load clicked!", 2000);
}

void MainWindow::handleRightButton()
{
    // 真·退出（带确认）
    auto ret = QMessageBox::question(this, "Exit", "Are you sure you want to exit?",
                                     QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        emit statusUpdateMessage("Exiting...", 1000);
        close();
    } else {
        emit statusUpdateMessage("Exit cancelled.", 2000);
    }
}

void MainWindow::onTreeClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    currentIndex = index;

    auto* item = static_cast<ModelPart*>(index.internalPointer());
    if (!item) return;

    QString name = item->data(0).toString();
    QString visible = item->data(1).toString();

    emit statusUpdateMessage(QString("Selected: %1 | visible = %2").arg(name, visible), 3000);
}

void MainWindow::handleOpenFileAction()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);;Text Files (*.txt);;All Files (*.*)")
        );

    if (fileName.isEmpty()) {
        emit statusUpdateMessage("Open File cancelled.", 2000);
        return;
    }

    if (!currentIndex.isValid()) {
        emit statusUpdateMessage("Select a tree item first, then Open File.", 4000);
        return;
    }

    auto* item = static_cast<ModelPart*>(currentIndex.internalPointer());
    if (!item) return;

    QString justName = QFileInfo(fileName).fileName();
    item->set(0, justName);

    // 刷新当前行
    QModelIndex left = partList->index(currentIndex.row(), 0, currentIndex.parent());
    QModelIndex right = partList->index(currentIndex.row(), 1, currentIndex.parent());
    emit partList->dataChanged(left, right);

    emit statusUpdateMessage(QString("Renamed selected item to: %1").arg(justName), 5000);
}

void MainWindow::on_actionItem_Options_triggered()
{
    if (!currentIndex.isValid()) {
        emit statusUpdateMessage("Right-click: select an item first.", 3000);
        return;
    }

    auto* item = static_cast<ModelPart*>(currentIndex.internalPointer());
    if (!item) return;

    OptionDialog dialog(this);

    // 如果你的 OptionDialog 已经有这些函数，就会把数据带进去/带出来
    // 如果还没有，也不影响先弹出来（至少右键有反应）
    if constexpr (true) {
        // 这两句如果你还没实现，会编译报错；报错就把报错截图发我，我再给你 optiondialog 的最终版
        dialog.setFromModelPart(item);

        if (dialog.exec() == QDialog::Accepted) {
            dialog.applyToModelPart(item);

            QModelIndex left = partList->index(currentIndex.row(), 0, currentIndex.parent());
            QModelIndex right = partList->index(currentIndex.row(), 1, currentIndex.parent());
            emit partList->dataChanged(left, right);

            emit statusUpdateMessage("Item updated from dialog.", 3000);
        } else {
            emit statusUpdateMessage("Dialog cancelled.", 2000);
        }
    }
}
