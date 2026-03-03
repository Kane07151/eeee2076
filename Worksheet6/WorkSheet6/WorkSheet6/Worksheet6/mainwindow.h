#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_pushButton_2_released(); // Options button

private:
    Ui::MainWindow *ui = nullptr;
    ModelPartList* partList = nullptr;
};

#endif // MAINWINDOW_H
