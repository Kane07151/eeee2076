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

public slots:
    void handleButton(); // Button 1

private slots:
    void on_pushButton_2_released(); // Optional: keep button 2 for options
    void onTreeViewContextMenuRequested(const QPoint& pos); // NEW: context menu

private:
    void openOptionsDialogForIndex(const QModelIndex& index); // helper

private:
    Ui::MainWindow *ui = nullptr;
    ModelPartList* partList = nullptr;
};

#endif // MAINWINDOW_H
