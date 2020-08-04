/***********************************************************************
 * $QT_BEGIN_LICENSE:GPL-EXCEPT$
 *
 * Stress-Strain Diagram Builder
 * A program for building strain diagrams according to a digital multimeter
 * Copyright (C) 2020 Raven-98
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 * *********************************************************************
 *  Author: Raven-98
 *    Date: 08.2020
 * Version: 0.4.0
 * *********************************************************************
 *
 * $QT_END_LICENSE$
***********************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QActionGroup>
#include <QTranslator>
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include <QPointer>

#include "infoprogram.h"
#include "tablewidget.h"
#include "dialogopenfile.h"
#include "plotwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void on_actionAbout_program_triggered();
    void on_actionEnglish_triggered();
    void on_actionRussian_triggered();
    void on_actionOpen_File_triggered();
    void addPlot();

private:
    Ui::MainWindow *ui;
    QSettings *sett;
    QActionGroup *actionLanguageGroup;
    QPointer<TableWidget> tableWidget;
    QPointer<PlotWidget> plotWidget;
    QTranslator languageTranslator;

    void set_actionLanguageGroupChecked(QString lang);
    void set_language(QString lang);
    QString get_language();
    void loadSubWindow(QWidget *widget);
    void addTable(QStringList list);
    QVector<QVector<double>> readData(QStringList data);
};
#endif // MAINWINDOW_H
