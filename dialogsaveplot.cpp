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

#include "dialogsaveplot.h"
#include "./ui_dialogsaveplot.h"

DialogSavePlot::DialogSavePlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSavePlot)
{
    ui->setupUi(this);

    sett = new QSettings(QCoreApplication::applicationDirPath() + "/ssdb.ini",QSettings::IniFormat);

    ui->lineEditName->setText(sett->value("DialogSavePlot/file_name").toString());
    ui->lineEditPath->setText(sett->value("DialogSavePlot/file_path").toString());

    ui->comboBoxFormat->addItems({"*.png","*.bmp","*.jpeg","*.pdf"});

    if (sett->value("DialogSavePlot/format").toString() == ".png")
    {
        ui->comboBoxFormat->setCurrentIndex(0);
    }
    else if (sett->value("DialogSavePlot/format").toString() == ".bmp")
    {
        ui->comboBoxFormat->setCurrentIndex(1);
    }
    else if (sett->value("DialogSavePlot/format").toString() == ".jpeg")
    {
        ui->comboBoxFormat->setCurrentIndex(2);
    }
    else if (sett->value("DialogSavePlot/format").toString() == ".pdf")
    {
        ui->comboBoxFormat->setCurrentIndex(3);
    }
}

DialogSavePlot::~DialogSavePlot()
{
    delete sett;
    delete ui;
}

QStringList DialogSavePlot::getInput()
{
    QString format;
    switch (ui->comboBoxFormat->currentIndex())
    {
        case 0:
            format = ".png";
            break;
        case 1:
            format = ".bmp";
            break;
        case 2:
            format = ".jpeg";
            break;
        case 3:
            format = ".pdf";
            break;
    }

    sett->setValue("DialogSavePlot/file_name",ui->lineEditName->text());
    sett->setValue("DialogSavePlot/file_path",ui->lineEditPath->text());
    sett->setValue("DialogSavePlot/format",format);

    return {ui->lineEditPath->text(),ui->lineEditName->text(),format};

}

void DialogSavePlot::on_pushButtonNameOpen_clicked()
{
    ui->lineEditName->setText(QFileDialog::getOpenFileUrl(this,tr("Open File"),tr("All")).fileName().section('.',0,0));
}

void DialogSavePlot::on_pushButtonPathOpen_clicked()
{
    ui->lineEditPath->setText(QFileDialog::getExistingDirectoryUrl(this).path());
}

void DialogSavePlot::accept()
{
    if (ui->lineEditName->text() != "")
    {
        if (ui->lineEditPath->text() != "")
        {

            done(Accepted);
        }
        else
        {
            QMessageBox::warning(this,tr("Warning"),tr("The \"Path\" field cannot be empty"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Warning"),tr("The \"File\" field cannot be empty"));
    }
}

void DialogSavePlot::on_buttonBox_accepted()
{
    if (ui->lineEditName->text() != "")
    {
        if (ui->lineEditPath->text() != "")
        {

            done(Accepted);
        }
        else
        {
            QMessageBox::warning(this,tr("Warning"),tr("The \"Path\" field cannot be empty"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Warning"),tr("The \"File\" field cannot be empty"));
    }
}

void DialogSavePlot::on_buttonBox_rejected()
{
    done(Rejected);
}
