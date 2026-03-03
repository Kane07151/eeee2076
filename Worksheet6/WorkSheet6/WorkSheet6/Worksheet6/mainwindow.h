#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

#include "ModelPartList.h"
#include "ModelPart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void statusUpdateMessage(const QString& message, int timeout);

private slots:
    void on_pushButton_released();     // NEW: Open File Dialog -> change item name
    void on_pushButton_2_released();   // Options dialog button
    void onTreeViewContextMenuRequested(const QPoint& pos); // Context menu
    void on_actionOpenFile_triggered();
private:
    void openOptionsDialogForIndex(const QModelIndex& index);

private:
    Ui::MainWindow *ui = nullptr;
    ModelPartList* partList = nullptr;
};

#endif // MAINWINDOW_H
