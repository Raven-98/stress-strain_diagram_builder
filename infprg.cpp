#include "infprg.h"
#include "ui_infprg.h"

infPrg::infPrg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::infPrg)
{
    ui->setupUi(this);
    QString *v = new QString(QT_VERSION_STR);
    ui->label_7->setText("Qt " + *v);
    delete v;
}

infPrg::~infPrg()
{
    delete ui;
}
