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

#include "tablewidget.h"
#include "./ui_tablewidget.h"

#include <QDebug>

TableWidget::TableWidget(QWidget *parent, QString winTitle) :
    QTableWidget(parent),
    ui(new Ui::TableWidget)
{
    ui->setupUi(this);
    setWindowTitle(winTitle.section('/', -1).section('.',0,0));

    fileDir = new QString(winTitle);
    fileDir->remove(winTitle.section('/', -1));
}

TableWidget::~TableWidget()
{
    delete fileDir;
    delete ui;
}

void TableWidget::set_Data(QVector<QVector<double>> data)
{
        int *vi = new int(0);
        for (QVector<double>::iterator it = data[0].begin(); it != data[0].end(); ++it)
        {
            insertRow(*vi);
            setItem(*vi,0,new QTableWidgetItem(QString::number(data[0][*vi])));
            setItem(*vi,1,new QTableWidgetItem(QString::number(data[1][*vi])));
            *vi = *vi + 1;
        }
        delete vi;

    setHorizontalHeaderItem(0,new QTableWidgetItem("ε, %"));
    setHorizontalHeaderItem(1,new QTableWidgetItem("σ, MPa"));
}

QVector<QVector<double>> TableWidget::get_Data()
{
    //if (hasFocus())
    {
        QVector<QVector<double>> d(2,QVector<double>(rowCount()));
        int *vi = new int(0);
        for (QVector<double>::iterator it = d[0].begin(); it != d[0].end(); ++it)
        {
            d[0][*vi] = item(*vi,0)->text().toDouble();
            d[1][*vi] = item(*vi,1)->text().toDouble();
            *vi = *vi + 1;
        }
        delete vi;
        return d;
    }
}

void TableWidget::save_Data()
{
    if (hasFocus())
    {
        DialogSaveData *dialogSaveFile = new DialogSaveData(this);
        switch(dialogSaveFile->exec())
        {
        case QDialog::Accepted:
            save(dialogSaveFile->getInput());
            break;
        case QDialog::Rejected:
            break;
        default:
            qDebug() << tr("QDialog: Unexpected result");
            break;
        }

        delete dialogSaveFile;
    }
}

void TableWidget::save(QStringList str)
{
    QFile *fil = new QFile(str.at(0)+"/"+str.at(1)+str.at(2));
    if (!fil->open(QIODevice::WriteOnly))
    {
        fil->close();
        QMessageBox::critical(this,tr("Error"),tr("Error")+": "+fil->errorString());
    }
    else
    {
        QTextStream *ts = new QTextStream(fil);
        *ts << "epsilon" << str.at(3) << "sigma [MPa]\n";
        for (int i = 0; i <= rowCount();i++)
        {
            if (item(i,0)!=nullptr & item(i,1)!=nullptr)
                if (item(i,0)->text()!="" & item(i,1)->text()!="")
                    *ts << item(i,0)->text().toDouble() << str.at(3) << item(i,1)->text().toDouble() << "\n";
        }
        fil->close();
        QMessageBox::information(this,"",tr("Saving was successful"));
    }
    delete fil;
}


