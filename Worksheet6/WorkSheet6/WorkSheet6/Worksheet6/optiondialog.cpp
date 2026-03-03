#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::optiondialog)   // ✅ 小写
{
    ui->setupUi(this);

    ui->spinR->setRange(0, 255);
    ui->spinG->setRange(0, 255);
    ui->spinB->setRange(0, 255);
}

OptionDialog::~OptionDialog()
{
    delete ui;
    ui = nullptr;
}

void OptionDialog::setFromModelPart(const ModelPart* part)
{
    if (!part) return;

    ui->lineEditName->setText(part->data(0).toString());

    const QString vis = part->data(1).toString().toLower();
    ui->checkVisible->setChecked(vis == "true" || vis == "1" || vis == "yes");

    ui->spinR->setValue(part->red());
    ui->spinG->setValue(part->green());
    ui->spinB->setValue(part->blue());
}

void OptionDialog::applyToModelPart(ModelPart* part) const
{
    if (!part) return;

    part->setData(0, ui->lineEditName->text());
    part->setData(1, ui->checkVisible->isChecked() ? QString("true") : QString("false"));
    part->setRGB(ui->spinR->value(), ui->spinG->value(), ui->spinB->value());
}
