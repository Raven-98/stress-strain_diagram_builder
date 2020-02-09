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

#include "qcustomplot/qcustomplot.h"

#include <QTranslator>
#include <QLibraryInfo>
#include <QActionGroup>

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
    void infoFile();
    void infoSave();

    void ButtFileName();
    void ButtPlots();
    void ButtPlotSave();
    void ButtDataSave();

    void edFileName();

    void slotMouseMove(QMouseEvent * event);

    void languageEnFunc();
    void languageRuFunc();

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

    double q_str_to_double(QString str);

    QCPItemTracer *tracer;

    QTranslator languageTranslator;
    QActionGroup *actionGroup;
    QString *language;

    void languagesFunc(QString *lang);
};

#endif // MAINWINDOW_H
