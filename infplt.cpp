#include "infplt.h"
#include "ui_infplt.h"

infPlt::infPlt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::infPlt)
{
    ui->setupUi(this);
}

infPlt::~infPlt()
{
    delete ui;
}
