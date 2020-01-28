#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMessageBox>
#include <QCoreApplication>
#include <QIcon>
#include <QPixmap>
#include <QSettings>
#include <QString>
#include <QPen>
#include <QFile>
#include <QTextStream>

#include "fast-cpp-csv-parser-master/csv.h"

#include <vector>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void infoPrg();
    void helpPrg();

    void ButtFileName();
    void ButtPlots();
    void ButtPlotSave();
    void ButtDataSave();

    void edFileName();

private:
    Ui::MainWindow *ui;
    QSettings *sett;

    double **ddd;
    int *max_index_i;

    struct dt
    {
        int time;
        double value;
        std::string unit;
    };
};

#endif // MAINWINDOW_H
