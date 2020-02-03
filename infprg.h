#ifndef INFPRG_H
#define INFPRG_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class infPrg;
}

class infPrg : public QDialog
{
    Q_OBJECT

public:
    explicit infPrg(QWidget *parent);
    ~infPrg();

private:
    Ui::infPrg *ui;
};

#endif // INFPRG_H
