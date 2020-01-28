#ifndef INFPLT_H
#define INFPLT_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class infPlt;
}

class infPlt : public QDialog
{
    Q_OBJECT

public:
    explicit infPlt(QWidget *parent);
    ~infPlt();

private:
    Ui::infPlt *ui;
};

#endif // INFPLT_H
