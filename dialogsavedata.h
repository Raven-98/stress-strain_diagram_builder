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

#ifndef DIALOGSAVEDATA_H
#define DIALOGSAVEDATA_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class DialogSaveData;
}
QT_END_NAMESPACE

class DialogSaveData : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSaveData(QWidget *parent = nullptr);
    ~DialogSaveData();
    QStringList getInput();

private slots:
    void on_pushButtonNameOpen_clicked();
    void on_pushButtonPathOpen_clicked();

private:
    Ui::DialogSaveData *ui;
    QSettings *sett;

    void accept();
};

#endif // DIALOGSAVEDATA_H
