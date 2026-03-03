#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

class ModelPart;

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

    void setFromModelPart(ModelPart* part);
    void applyToModelPart(ModelPart* part);

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
