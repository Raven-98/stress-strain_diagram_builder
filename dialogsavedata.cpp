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

#include "dialogsavedata.h"
#include "ui_dialogsavedata.h"

DialogSaveData::DialogSaveData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSaveData)
{
    ui->setupUi(this);

    sett = new QSettings(QCoreApplication::applicationDirPath() + "/ssdb.ini",QSettings::IniFormat);

    ui->lineEditName->setText(sett->value("DialogSaveData/file_name").toString());
    ui->lineEditPath->setText(sett->value("DialogSaveData/file_path").toString());

    ui->comboBoxFormat->addItems({"*.csv","*.txt"});
    ui->comboBoxDelimiter->addItems({tr("Comma"),tr("Tab step"),tr("Semicolon"),tr("Space")});

    if (sett->value("DialogSaveData/delimiter").toString() == ",")
    {
        ui->comboBoxDelimiter->setCurrentIndex(0);
    }
    else if (sett->value("DialogSaveData/delimiter").toString() == "\t")
    {
        ui->comboBoxDelimiter->setCurrentIndex(1);
    }
    else if (sett->value("DialogSaveData/delimiter").toString() == ";")
    {
        ui->comboBoxDelimiter->setCurrentIndex(2);
    }
    else if (sett->value("DialogSaveData/delimiter").toString() == " ")
    {
        ui->comboBoxDelimiter->setCurrentIndex(3);
    }

    if (sett->value("DialogSaveData/format").toString() == ".csv")
    {
        ui->comboBoxFormat->setCurrentIndex(0);
    }
    else if (sett->value("DialogSaveData/format").toString() == ".txt")
    {
        ui->comboBoxFormat->setCurrentIndex(1);
    }
}

DialogSaveData::~DialogSaveData()
{
    delete sett;
    delete ui;
}

void DialogSaveData::accept()
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

QStringList DialogSaveData::getInput()
{
    QString format;
    switch (ui->comboBoxFormat->currentIndex())
    {
        case 0:
            format = ".csv";
            break;
        case 1:
            format = ".txt";
            break;
    }

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

    sett->setValue("DialogSaveData/file_name",ui->lineEditName->text());
    sett->setValue("DialogSaveData/file_path",ui->lineEditPath->text());
    sett->setValue("DialogSaveData/format",format);
    sett->setValue("DialogSaveData/delimiter",delimiter);

    return {ui->lineEditPath->text(),ui->lineEditName->text(),format,delimiter};
}

void DialogSaveData::on_pushButtonNameOpen_clicked()
{
    ui->lineEditName->setText(QFileDialog::getOpenFileUrl(this,tr("Open File"),tr("All")).fileName().section('.',0,0));
}

void DialogSaveData::on_pushButtonPathOpen_clicked()
{
    ui->lineEditPath->setText(QFileDialog::getExistingDirectoryUrl(this).path());
}
