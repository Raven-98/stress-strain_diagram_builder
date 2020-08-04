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

#include "plotwidget.h"
#include "./ui_plotwidget.h"

#include <QDebug>

PlotWidget::PlotWidget(QWidget *parent) :
    QCustomPlot(parent),
    ui(new Ui::PlotWidget)
{
    ui->setupUi(this);
    tracer = new QCPItemTracer(this);
    connect(this,&QCustomPlot::mouseMove,this,&PlotWidget::slotMouseMove);
}

PlotWidget::~PlotWidget()
{
    delete ui;
}

void PlotWidget::set_Data(QVector<QVector<double>> data, QString title)
{
    setWindowTitle(title);
    clearGraphs();
    addGraph();
    if (data[0].size() != 0 &  data[1].size() != 0)
    {
        graph(0)->setData(data[0],data[1]);

        xAxis->setRange(0,calculate_limit(*std::max_element(data[0].begin(),data[0].end())));
        yAxis->setRange(0,calculate_limit(*std::max_element(data[1].begin(),data[1].end())));

        xAxis->setLabel("ε, %");
        yAxis->setLabel("σ, MPa");

        xAxis2->setVisible(true);
        xAxis2->setTicks(false);
        xAxis2->setTickLabels(false);
        yAxis2->setVisible(true);
        yAxis2->setTicks(false);
        yAxis2->setTickLabels(false);

        xAxis->grid()->setSubGridVisible(true);
        yAxis->grid()->setSubGridVisible(true);
        xAxis->grid()->setPen(QPen(QColor(Qt::gray),1,Qt::DotLine));
        yAxis->grid()->setPen(QPen(QColor(Qt::gray),1,Qt::DotLine));

        setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

        tracer->setGraph(graph(0));
    }
    replot();
}

void PlotWidget::save_Plot()
{
    if (hasFocus())
    {
        DialogSavePlot *dialogSavePlot = new DialogSavePlot(this);
        switch(dialogSavePlot->exec())
        {
        case QDialog::Accepted:
            save(dialogSavePlot->getInput());
            break;
        case QDialog::Rejected:
            break;
        default:
            qDebug() << tr("QDialog: Unexpected result");
            break;
        }

        delete dialogSavePlot;
    }
}

double PlotWidget::calculate_limit(double value)
{
    return (value*102)/100;
}

void PlotWidget::save(QStringList str)
{
    if (str.at(2) == ".png")
    {
        QFile *file = new QFile(str.at(0)+"/"+str.at(1)+str.at(2));
        if (!file->open(QIODevice::WriteOnly))
        {
            file->close();
            QMessageBox::critical(this,tr("Error"),tr("Error")+": "+file->errorString());
        }
        else
        {
            savePng(str.at(0)+"/"+str.at(1)+str.at(2));
            file->close();
            QMessageBox::information(this,"",tr("Saving was successful"));
        }
    }
    else if (str.at(2) == ".bmp")
    {
        QFile *file = new QFile(str.at(0)+"/"+str.at(1)+str.at(2));
        if (!file->open(QIODevice::WriteOnly))
        {
            file->close();
            QMessageBox::critical(this,tr("Error"),tr("Error")+": "+file->errorString());
        }
        else
        {
            saveBmp(str.at(0)+"/"+str.at(1)+str.at(2));
            file->close();
            QMessageBox::information(this,"",tr("Saving was successful"));
        }
    }
    else if (str.at(2) == ".jpeg")
    {
        QFile *file = new QFile(str.at(0)+"/"+str.at(1)+str.at(2));
        if (!file->open(QIODevice::WriteOnly))
        {
            file->close();
            QMessageBox::critical(this,tr("Error"),tr("Error")+": "+file->errorString());
        }
        else
        {
            saveJpg(str.at(0)+"/"+str.at(1)+str.at(2));
            file->close();
            QMessageBox::information(this,"",tr("Saving was successful"));
        }
    }
    else if (str.at(2) == ".pdf")
    {
        QFile *file = new QFile(str.at(0)+"/"+str.at(1)+str.at(2));
        if (!file->open(QIODevice::WriteOnly))
        {
            file->close();
            QMessageBox::critical(this,tr("Error"),tr("Error")+": "+file->errorString());
        }
        else
        {
            savePdf(str.at(0)+"/"+str.at(1)+str.at(2));
            file->close();
            QMessageBox::information(this,"",tr("Saving was successful"));
        }
    }

}

void PlotWidget::slotMouseMove(QMouseEvent *event)
{
    double coordX = xAxis->pixelToCoord(event->pos().x());
    tracer->setGraphKey(coordX);
    ui->labelTracer->setText("ε: "+QString::number(tracer->position->key())+" σ: "+QString::number(tracer->position->value()));
    replot();
}
