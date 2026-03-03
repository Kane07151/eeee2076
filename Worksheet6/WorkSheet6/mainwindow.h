#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "ModelPartList.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void statusUpdateMessage(const QString &message, int timeout);

private slots:
    void handleLeftButton();
    void handleRightButton();
    void onTreeClicked(const QModelIndex &index);

    void handleOpenFileAction();              // 菜单/工具栏 Open File
    void on_actionItem_Options_triggered();   // 右键 Item Options（Designer action）

private:
    Ui::MainWindow *ui;
    ModelPartList* partList = nullptr;

    QAction* actionOpen_File = nullptr;

    QModelIndex currentIndex; // 记录当前选中项，给 OpenFile / 右键编辑用

    void buildDemoTree();
};

#endif // MAINWINDOW_H
