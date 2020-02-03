#include "infprg.h"
#include "ui_infprg.h"

#include <QDebug>

infPrg::infPrg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::infPrg)
{
    ui->setupUi(this);
    QString *v = new QString(QT_VERSION_STR);
    *v = "<html><head/><body><p><br/>Qt " + *v + "</p><p><a href=\"https://www.qt.io/\"><span style=\" text-decoration: underline; color:#2980b9;\">https://www.qt.io/</span></a></p></body></html>";
    ui->label_7->setText(*v);
    delete v;
}

infPrg::~infPrg()
{
    delete ui;
}
