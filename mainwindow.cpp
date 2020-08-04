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

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sett = new QSettings(QCoreApplication::applicationDirPath() + "/ssdb.ini",QSettings::IniFormat);

    actionLanguageGroup = new QActionGroup(this);
    actionLanguageGroup->addAction(ui->actionEnglish);
    actionLanguageGroup->addAction(ui->actionRussian);

    set_actionLanguageGroupChecked(sett->value("SETTING/language").toString());
    set_language(sett->value("SETTING/language").toString());

    connect(ui->actionBuild_a_chart,&QAction::triggered,this,&MainWindow::addPlot);
}

MainWindow::~MainWindow()
{
    sett->setValue("SETTING/language",get_language());

    delete actionLanguageGroup;
    delete sett;
    delete ui;
}

void MainWindow::set_actionLanguageGroupChecked(QString lang)
{
    if (lang == "Ru")
        ui->actionRussian->setChecked(true);
    else
        ui->actionEnglish->setChecked(true);
}

void MainWindow::set_language(QString lang)
{
    lang.resize(2);
//    QTranslator languageTranslator;
    languageTranslator.load(":/translations/ssdb_" + lang);
    qApp->installTranslator(&languageTranslator);
    ui->retranslateUi(this);
}

QString MainWindow::get_language()
{
    QString l = actionLanguageGroup->checkedAction()->text();
    l.resize(2);
    return l;
}

void MainWindow::loadSubWindow(QWidget *widget)
{
    auto window = ui->centralwidget->addSubWindow(widget);
    window->setWindowTitle(widget->windowTitle());
    window->resize(widget->geometry().width(),widget->geometry().height());
    window->show();
}

void MainWindow::addTable(QStringList list)
{
    tableWidget = new TableWidget(this,list.at(0));

    if (tableWidget)
    {
        tableWidget->set_Data(readData(list));
        connect(ui->actionSave_data,&QAction::triggered,tableWidget,&TableWidget::save_Data);
    }

    loadSubWindow(tableWidget);
}

QVector<QVector<double> > MainWindow::readData(QStringList data)
{
    QStandardItemModel *csvModel = new QStandardItemModel(this);

    QFile *file = new QFile(data.at(0));
    if ( !file->open(QFile::ReadOnly | QFile::Text) ) {
            QMessageBox::critical(this,tr("Error"),tr("File not exists"));
    }
    else
    {
        QTextStream *in = new QTextStream(file);
        while (!in->atEnd())
        {
            QString *line = new QString(in->readLine());
            QList<QStandardItem *> standardItemsList;

            for (QString item : line->split(data.at(6))) {
                standardItemsList.append(new QStandardItem(item));
            }
            delete line;
            csvModel->insertRow(csvModel->rowCount(), standardItemsList);
            standardItemsList.clear();
        }
        delete in;
        file->close();
    }
    delete file;

    int *size = new int(csvModel->rowCount()-1);

    int *index_value = new int(-1);
    int *index_until = new int(-1);
    int *index_time = new int(-1);

    for (int i = 0; i < csvModel->columnCount();i++)
    {
        if (csvModel->item(0,i)->text() == "Time")
            *index_time = i;
        if (csvModel->item(0,i)->text() == "Value")
            *index_value = i;
        if (csvModel->item(0,i)->text() == "Unit")
            *index_until = i;
    }

    if (*index_time != -1 && csvModel->item(0,*index_time)->text() == "Time")
    {
        if (*index_value != -1 && csvModel->item(0,*index_value)->text() == "Value")
        {
            if (*index_until != -1 && csvModel->item(0,*index_until)->text() == "Unit")
            {
                int *index_u = new int;
                for (int i = 1; i <= *size; i++)
                {
                    if (csvModel->item(i,*index_value)->text().toDouble() >= 0)
                    {
                        *index_u = i;
                        break;
                    }
                }

                int *max_index_u = new int(*size - *index_u);

                double *k = new double(data.at(1).toDouble());
                double *a = new double(data.at(2).toDouble());
                double *b = new double(data.at(3).toDouble());
                double *l0 = new double(data.at(4).toDouble());
                double *v = new double(data.at(5).toDouble());

                QVector<QVector<double>> data(2,QVector<double>(*max_index_u+1));

                for (int i = 0;i <= *max_index_u;i++)
                {
                    data[0][i] = (*v * ((csvModel->item(i+*index_u,*index_time)->text().section(':',2).toDouble()
                                        + csvModel->item(i+*index_u,*index_time)->text().section(':',1,1).toDouble()*60
                                        + csvModel->item(i+*index_u,*index_time)->text().section(':',0,0).toDouble()*3600)
                                        -(csvModel->item(*index_u,*index_time)->text().section(':',2).toDouble()
                                        + csvModel->item(*index_u,*index_time)->text().section(':',1,1).toDouble()*60
                                        + csvModel->item(*index_u,*index_time)->text().section(':',0,0).toDouble()*3600)) / *l0) * 100.;

                    if (csvModel->item(i+*index_u,*index_until)->text() == "mV")
                    {
                        data[1][i] = (csvModel->item(i+*index_u,*index_value)->text().toDouble() * *k) / (*a * *b) * 9.8;
                    }
                    else if(csvModel->item(i+*index_u,*index_until)->text() == "V")
                    {
                        data[1][i] = ((csvModel->item(i+*index_u,*index_value)->text().toDouble() * 1000.) * *k) / (*a * *b) * 9.8;
                    }
                }

                delete a;
                delete b;
                delete k;
                delete l0;
                delete v;
                delete index_u;

                return data;
            }
            else
            {
                QMessageBox::critical(this,"Error","Column \"Time\" not found");
                QVector<QVector<double>> data(2,QVector<double>(0));
                return data;
            }
        }
        else
        {
            QMessageBox::critical(this,"Error","Column \"Value\" not found");
            QVector<QVector<double>> data(2,QVector<double>(0));
            return data;
        }
    }
    else
    {
        QMessageBox::critical(this,"Error","Column \"Unit\" not found");
        QVector<QVector<double>> data(2,QVector<double>(0));
        return data;
    }
}

void MainWindow::addPlot()
{

    if (ui->centralwidget->activeSubWindow() != nullptr)
    {
        if (QString(ui->centralwidget->activeSubWindow()->widget()->metaObject()->className()) == "TableWidget")
        {
                if (ui->centralwidget->activeSubWindow()->widget()->hasFocus())
                {
                    plotWidget = new PlotWidget(this);

                    if (plotWidget)
                    {
                        plotWidget->set_Data(qobject_cast<TableWidget *>(ui->centralwidget->activeSubWindow()->widget())->get_Data(),ui->centralwidget->activeSubWindow()->widget()->windowTitle());
                        connect(ui->actionSave_chart,&QAction::triggered,plotWidget,&PlotWidget::save_Plot);
                    }

                    loadSubWindow(plotWidget);
                }
//          }
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionAbout_program_triggered()
{
    InfoProgram *infoProgram = new InfoProgram(this);
    infoProgram->show();
}

void MainWindow::on_actionEnglish_triggered()
{
    set_language(ui->actionEnglish->text());
}

void MainWindow::on_actionRussian_triggered()
{
    set_language(ui->actionRussian->text());
}

void MainWindow::on_actionOpen_File_triggered()
{
    DialogOpenFile *dialogOpenFile = new DialogOpenFile(this);
    switch(dialogOpenFile->exec())
    {
    case QDialog::Accepted:
        addTable(dialogOpenFile->getInput());
        break;
    case QDialog::Rejected:
        break;
    default:
        qDebug() << tr("QDialog: Unexpected result");
        break;
    }
    delete dialogOpenFile;
}
