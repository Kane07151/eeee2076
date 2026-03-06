#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

#include "ModelPartList.h"
#include "ModelPart.h"

#include <vtkSmartPointer.h>

class vtkRenderer;
class vtkGenericOpenGLRenderWindow;

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
    void on_pushButton_released();
    void on_pushButton_2_released();
    void on_actionOpenFile_triggered();
    void onTreeViewContextMenuRequested(const QPoint& pos);

private:
    void openOptionsDialogForIndex(const QModelIndex& index);
    void doOpenFileAndRenameSelectedItem();

private:
    Ui::MainWindow *ui = nullptr;
    ModelPartList* partList = nullptr;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
};

#endif // MAINWINDOW_H
