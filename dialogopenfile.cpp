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
 *    Date: 04.08.2020
 * Version: 0.4.0
 * *********************************************************************
 *
 * $QT_END_LICENSE$
***********************************************************************/

#include "dialogopenfile.h"
#include "./ui_dialogopenfile.h"

DialogOpenFile::DialogOpenFile(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogOpenFile)
{
    ui->setupUi(this);

    sett = new QSettings(QCoreApplication::applicationDirPath() + "/ssdb.ini",QSettings::IniFormat);

    ui->lineEditFile->setText(sett->value("DialogOpenFile/file").toString());
    ui->lineEdit_k->setText(sett->value("DialogOpenFile/k").toString());
    ui->lineEdit_a->setText(sett->value("DialogOpenFile/a").toString());
    ui->lineEdit_b->setText(sett->value("DialogOpenFile/b").toString());
    ui->lineEdit_l0->setText(sett->value("DialogOpenFile/l0").toString());
    ui->lineEdit_v->setText(sett->value("DialogOpenFile/v").toString());

    ui->comboBoxDelimiter->addItems({tr("Comma"),tr("Tab step"),tr("Semicolon"),tr("Space")});

    if (sett->value("DialogOpenFile/delimiter").toString() == ",")
    {
        ui->comboBoxDelimiter->setCurrentIndex(0);
    }
    else if (sett->value("DialogOpenFile/delimiter").toString() == "\t")
    {
        ui->comboBoxDelimiter->setCurrentIndex(1);
    }
    else if (sett->value("DialogOpenFile/delimiter").toString() == ";")
    {
        ui->comboBoxDelimiter->setCurrentIndex(2);
    }
    else if (sett->value("DialogOpenFile/delimiter").toString() == " ")
    {
        ui->comboBoxDelimiter->setCurrentIndex(3);
    }

    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&DialogOpenFile::accept);
    connect(ui->buttonBox,&QDialogButtonBox::rejected,this,[=](){done(Rejected);});
    connect(ui->pushButtonOpen,&QPushButton::clicked,this,&DialogOpenFile::openFile);
}

DialogOpenFile::~DialogOpenFile()
{
    delete sett;
    delete ui;
}

void DialogOpenFile::openFile()
{
    ui->lineEditFile->setText(QFileDialog::getOpenFileName(this,tr("Open file"),QDir::homePath(),tr("Text files(*.txt);;All files(*.*)")));
}

void DialogOpenFile::accept()
{
    if (ui->lineEditFile->text() != "")
    {
        if (ui->lineEdit_k->text() != "")
        {
            if (ui->lineEdit_a->text() != "")
            {
                if (ui->lineEdit_b->text() != "")
                {
                    if (ui->lineEdit_l0->text() != "")
                    {
                        if (ui->lineEdit_v->text() != "")
                        {
                            done(Accepted);
                            //QDialog::accept();
                        }
                        else
                        {
                            QMessageBox::warning(this,tr("Warning"),tr("The \"v\" field cannot be empty"));
                        }
                    }
                    else
                    {
                        QMessageBox::warning(this,tr("Warning"),tr("<html><head/><body><p>The \"l<span style=\" vertical-align:sub;\">0</span>\" field cannot be empty</p></body></html>"));
                    }
                }
                else
                {
                    QMessageBox::warning(this,tr("Warning"),tr("The \"b\" field cannot be empty"));
                }
            }
            else
            {
                QMessageBox::warning(this,tr("Warning"),tr("The \"a\" field cannot be empty"));
            }
        }
        else
        {
            QMessageBox::warning(this,tr("Warning"),tr("The \"k\" field cannot be empty"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Warning"),tr("The \"File\" field cannot be empty"));
    }
}

QStringList DialogOpenFile::getInput()
{
    QString delimiter;
    switch (ui->comboBoxDelimiter->currentIndex())
    {
        case 0:
            delimiter = ",";
            break;
        case 1:
            delimiter = "\t";
            break;
        case 2:
            delimiter = ";";
            break;
        case 3:
            delimiter = " ";
            break;
    }

    sett->setValue("DialogOpenFile/file",ui->lineEditFile->text());
    sett->setValue("DialogOpenFile/k",QString::number(ui->lineEdit_k->text().replace(",",".").toDouble()));
    sett->setValue("DialogOpenFile/a",QString::number(ui->lineEdit_a->text().replace(",",".").toDouble()));
    sett->setValue("DialogOpenFile/b",QString::number(ui->lineEdit_b->text().replace(",",".").toDouble()));
    sett->setValue("DialogOpenFile/l0",QString::number(ui->lineEdit_l0->text().replace(",",".").toDouble()));
    sett->setValue("DialogOpenFile/v",QString::number(ui->lineEdit_v->text().replace(",",".").toDouble()));
    sett->setValue("DialogOpenFile/delimiter",delimiter);

    return {ui->lineEditFile->text(),ui->lineEdit_k->text().replace(",","."),ui->lineEdit_a->text().replace(",","."),ui->lineEdit_b->text().replace(",","."),ui->lineEdit_l0->text().replace(",","."),ui->lineEdit_v->text().replace(",","."),delimiter};
}
