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

public slots:
    void handleButton();              // Button 1 handler (Exercise 2/3)

private slots:
    void on_pushButton_2_released();  // Options dialog button

private:
    Ui::MainWindow *ui = nullptr;
    ModelPartList* partList = nullptr;
};

#endif // MAINWINDOW_H
