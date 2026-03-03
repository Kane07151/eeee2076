#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include "ModelPart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class optiondialog; }  // ✅ 小写，和 .ui 的 <class> 一致
QT_END_NAMESPACE

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

    void setFromModelPart(const ModelPart* part);
    void applyToModelPart(ModelPart* part) const;

private:
    Ui::optiondialog *ui = nullptr;   // ✅ 小写
};

#endif // OPTIONDIALOG_H
