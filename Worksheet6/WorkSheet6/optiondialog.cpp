#include "optiondialog.h"
#include "ui_optiondialog.h"

#include "ModelPart.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);

    // 0~255 RGB
    ui->spinBoxR->setRange(0, 255);
    ui->spinBoxG->setRange(0, 255);
    ui->spinBoxB->setRange(0, 255);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::setFromModelPart(ModelPart *part)
{
    if (!part) return;

    ui->lineEditName->setText(part->data(0).toString());

    ui->spinBoxR->setValue(static_cast<int>(part->getColourR()));
    ui->spinBoxG->setValue(static_cast<int>(part->getColourG()));
    ui->spinBoxB->setValue(static_cast<int>(part->getColourB()));

    ui->checkBoxVisible->setChecked(part->visible());
}

void OptionDialog::applyToModelPart(ModelPart *part)
{
    if (!part) return;

    part->set(0, ui->lineEditName->text());

    part->setColour(
        static_cast<unsigned char>(ui->spinBoxR->value()),
        static_cast<unsigned char>(ui->spinBoxG->value()),
        static_cast<unsigned char>(ui->spinBoxB->value())
        );

    part->setVisible(ui->checkBoxVisible->isChecked());

    // 让树里第二列继续显示 true/false（和你现在一致）
    part->set(1, ui->checkBoxVisible->isChecked() ? "true" : "false");
}
