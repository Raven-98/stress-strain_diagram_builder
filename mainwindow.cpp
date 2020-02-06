#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "infprg.h"
#include "infplt.h"

//#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->lineEdit->setFocus();

    sett = new QSettings(QCoreApplication::applicationDirPath() + "/ssdb.ini",QSettings::IniFormat);

    resize(sett->value("SETTING/mw_width").toInt(),sett->value("SETTING/mw_height").toInt());

    ui->pushButton->setIcon(QIcon(QPixmap(":/img/im001.png")));

    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);

    ui->lineEdit_2->setText(sett->value("SETTING/k").toString());
    ui->lineEdit_3->setText(sett->value("SETTING/a").toString());
    ui->lineEdit_4->setText(sett->value("SETTING/b").toString());
    ui->lineEdit_5->setText(sett->value("SETTING/l0").toString());
    ui->lineEdit_6->setText(sett->value("SETTING/v").toString());

    connect(ui->action_1,&QAction::triggered,this,qApp->quit);
    connect(ui->action_2,&QAction::triggered,this,&MainWindow::helpPrg);
    connect(ui->action_3,&QAction::triggered,this,&MainWindow::infoPrg);
    connect(ui->action_4,&QAction::triggered,this,&MainWindow::infoFile);
    connect(ui->action_5,&QAction::triggered,this,&MainWindow::infoSave);

    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::ButtFileName);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::ButtPlots);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::ButtPlotSave);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&MainWindow::ButtDataSave);

    connect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::edFileName);
    connect(ui->lineEdit_2,&QLineEdit::returnPressed,this,&MainWindow::ButtPlots);
    connect(ui->lineEdit_3,&QLineEdit::returnPressed,this,&MainWindow::ButtPlots);
    connect(ui->lineEdit_4,&QLineEdit::returnPressed,this,&MainWindow::ButtPlots);
    connect(ui->lineEdit_5,&QLineEdit::returnPressed,this,&MainWindow::ButtPlots);
    connect(ui->lineEdit_6,&QLineEdit::returnPressed,this,&MainWindow::ButtPlots);

    ddd = new double*[2];
    for (int i = 0;i < 2;i++)
    {
        ddd[i] = new double[1];
    }
    max_index_i = new int;

    tracer = new QCPItemTracer(ui->customPlot);
    connect(ui->customPlot,&QCustomPlot::mouseMove,this,&MainWindow::slotMouseMove);
}

void MainWindow::infoPrg()
{
    infPrg *infprg = new infPrg(this);

    infprg->show();
}

void MainWindow::helpPrg()
{
    infPlt *infplt = new infPlt(this);
    infplt->show();
}

void MainWindow::infoFile()
{
    QMessageBox ms;
    ms.setWindowTitle("Работа с файлами");
    ms.setText("При открытии файла желательно избегать символов кириллицы.\n\nДля корректного чтения файла и обработки данных из него желательно, чтобы файл имел формат txt. В файле обязательно должны быть следующие колонки: Time, Value и Unit; также все колонки должны быть разделены шагом табуляции.");
    ms.exec();
}

void MainWindow::infoSave()
{
    QMessageBox ms;
    ms.setWindowTitle("Сохранение данных");
    ms.setText("Сохранение диаграммы и обработаных данных производится по пути исходного файла.");
    ms.exec();
}

void MainWindow::edFileName()
{
    if (ui->lineEdit->text() == "")
            ButtFileName();
        else
            ButtPlots();
}

void MainWindow::ButtFileName()
{
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);

    QString *text = new QString(QFileDialog::getOpenFileName(this,"Открыть файл","","Text Files (*.txt);;All Files (*.*)"));

    ui->lineEdit->setText(*text);

    ui->statusBar->showMessage("Открыт файл: " + *text);

    delete text;

    ui->lineEdit->setFocus();
}

void MainWindow::ButtPlots()
{
    if (ui->lineEdit->text() == "")
    {
        QMessageBox::critical(this,"Ошибка","Поле \"Файл\" не может быть пустым!");
    }
    else if (ui->lineEdit_2->text() == "")
    {
        QMessageBox::critical(this,"Ошибка","Поле \"k\" не может быть пустым!");
    }
    else if (ui->lineEdit_3->text() == "")
    {
        QMessageBox::critical(this,"Ошибка","Поле \"a\" не может быть пустым!");
    }
    else if (ui->lineEdit_4->text() == "")
    {
            QMessageBox::critical(this,"Ошибка","Поле \"b\" не может быть пустым!");
    }
    else if (ui->lineEdit_5->text() == "")
    {
        QMessageBox::critical(this,"Ошибка","Поле \"l0\" не может быть пустым!");
    }
    else if (ui->lineEdit_6->text() == "")
    {
        QMessageBox::critical(this,"Ошибка","Поле \"v\" не может быть пустым!");
    }
    else
    {
        QString *fileName = new QString(ui->lineEdit->text());

        try
        {
            std::vector<dt> data;
            dt d;
            ui->statusBar->showMessage("Считывание данных...");
            io::CSVReader<3,io::trim_chars<' '>,io::no_quote_escape<'\t'>,io::single_line_comment<'#'>> file(fileName->toUtf8().constData());
            try
            {
                file.read_header(io::ignore_extra_column,"Time","Value","Unit");
                std::string Time;
                double Value;
                std::string Unit;
                while (file.read_row(Time,Value,Unit))
                {
                    if (Time.length() == 8)
                    {
                        d.time = std::stoi(Time.substr(0,2))*3600+std::stoi(Time.substr(3,2))*60+std::stoi(Time.substr(6,2));
                    } else if (Time.length() == 7)
                    {
                        d.time = std::stoi(Time.substr(0,1))*3600+std::stoi(Time.substr(2,2))*60+std::stoi(Time.substr(5,2));
                    }
                    d.value = Value;
                    d.unit = Unit;
                    data.push_back(d);
                }
                ui->statusBar->showMessage("Завершено");

                ui->statusBar->showMessage("Поиск начального значения...");
                unsigned long *index_u = new unsigned long();
                for (unsigned long i = 0;i < data.size();++i)
                {
                    if (data[i].value >= 0)
                    {
                        *index_u = i;
                        break;
                    }
                }
                ui->statusBar->showMessage("Завершено");

                 ui->statusBar->showMessage("Обработка данных...");
                unsigned long *max_index_u = new unsigned long(data.size() - *index_u);

                int *time = new int[*max_index_u];
                double *value = new double[*max_index_u];


                for (unsigned long i = 0;i < *max_index_u;++i)
                {
                    time[i] = data[i + *index_u].time - data[*index_u].time;
                    if (data[i + *index_u].unit == "mV")
                    {
                        value[i] = data[i + *index_u].value;
                    }
                    else if (data[i + *index_u].unit == "V")
                    {
                        value[i] = data[i + *index_u].value * 1000.;
                    }
                }

                data.clear();

                max_index_i = new int(static_cast<int>(*max_index_u));
                delete index_u;
                delete max_index_u;

                double *a = new double(q_str_to_double(ui->lineEdit_3->text()));
                double *b = new double(q_str_to_double(ui->lineEdit_4->text()));
                double *S = new double(*a * *b);
                delete a;
                delete b;

                double *k = new double(q_str_to_double(ui->lineEdit_2->text()));
                double *l0 = new double(q_str_to_double(ui->lineEdit_5->text()));
                double *v = new double(q_str_to_double(ui->lineEdit_6->text()));

                QVector<double> epsilon(*max_index_i), sigma(*max_index_i);
                for (int i = 0;i < *max_index_i;i++)
                {
                    epsilon[i] = (*v * time[i] / *l0) * 100.;
                    sigma[i] = (value[i] * *k) / *S * 9.8;
                }

                delete [] time;
                delete [] value;
                delete k;
                delete l0;
                delete v;
                ui->statusBar->showMessage("Завершено");

                ui->statusBar->showMessage("Построение диаграммы...");
                ui->customPlot->clearGraphs();
                ui->customPlot->addGraph();

                ui->customPlot->graph(0)->setData(epsilon,sigma);

                ui->customPlot->xAxis->setLabel("ε, %");
                ui->customPlot->yAxis->setLabel("σ, MPa");

                double *max_x = new double(epsilon[0]);
                double *max_y = new double(sigma[0]);
                for (int i = 1; i < *max_index_i; i++)
                {
                    if (epsilon[i] > *max_x) {
                        max_x = new double(epsilon[i]);
                    }
                    if (sigma[i] > *max_y) {
                        max_y = new double(sigma[i]);
                    }
                }

                ddd = new double*[2];
                for (int i = 0;i < 2;i++)
                {
                    ddd[i] = new double[*max_index_i];
                }
                for (int i = 0;i < *max_index_i;++i)
                {
                    ddd[0][i] = epsilon[i];
                    ddd[1][i] = sigma[i];
                }

                max_x = new double(*max_x + *max_x/50.);
                max_y = new double(*max_y + *max_y/50.);
                ui->customPlot->xAxis->setRange(0,*max_x);
                ui->customPlot->yAxis->setRange(0,*max_y);

                QPen *pen = new QPen;
                pen->setWidth(2);
                pen->setColor(QColor(0,100,180));
                ui->customPlot->graph(0)->setPen(*pen);
                delete pen;

                ui->customPlot->xAxis2->setVisible(true);
                ui->customPlot->xAxis2->setTickLabels(false);
                ui->customPlot->yAxis2->setVisible(true);
                ui->customPlot->yAxis2->setTickLabels(false);

                ui->customPlot->xAxis->grid()->setSubGridVisible(true);
                ui->customPlot->yAxis->grid()->setSubGridVisible(true);
                ui->customPlot->xAxis->grid()->setPen(QPen(QColor(Qt::gray),1,Qt::DotLine));
                ui->customPlot->yAxis->grid()->setPen(QPen(QColor(Qt::gray),1,Qt::DotLine));

                ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

                tracer->setGraph(ui->customPlot->graph(0));

                //ui->customPlot->setOpenGl(true,16);

                ui->customPlot->replot();
                ui->statusBar->showMessage("Завершено");

                epsilon.clear();
                sigma.clear();

                ui->pushButton_3->setEnabled(true);
                ui->pushButton_4->setEnabled(true);
            }
            catch (io::error::missing_column_in_header)
            {
                ui->statusBar->showMessage("Нет требуемых колонок для чтения");
                QMessageBox::critical(this,"Ошибка","Неверная структура файла");
            }
        }
        catch (io::error::can_not_open_file)
        {
            ui->statusBar->showMessage("Файл " + *fileName + " не смог быть открытым");
            QMessageBox::critical(this,"Ошибка","Нет такого файла или каталога");
        }

        delete fileName;
    }
    ui->lineEdit->setFocus();
}

void MainWindow::ButtPlotSave()
{
    tracer->setVisible(false);

    QString *s = new QString(ui->lineEdit->text());
    int *f = new int(s->length());

    s->remove(*f-4,4);
    delete f;

    *s = *s + "_plot.png";

    QFile *fil = new QFile(*s);

    if (!fil->open(QIODevice::WriteOnly))
    {
        fil->close();
        //qDebug() << fil->errorString();
        ui->statusBar->showMessage("Ошибка: " +fil->errorString());
    }
    else
    {
        ui->customPlot->savePng(*s,1181,866);
        fil->close();
        ui->statusBar->showMessage("Файл " + *s + " успешно сохранен");
        QMessageBox::information(this,"","Сохранение прошло успешно");
    }
    delete s;
    delete fil;

    ui->lineEdit->setFocus();

    tracer->setVisible(true);
}

void MainWindow::ButtDataSave()
{
    QString *s = new QString(ui->lineEdit->text());
    int *f = new int(s->length());

    s->remove(*f-4,4);
    delete f;

    *s = *s + "_plot_data.txt";

    QFile *fil = new QFile(*s);
    if (!fil->open(QIODevice::WriteOnly))
    {
        fil->close();
        //qDebug() << fil->errorString();
        ui->statusBar->showMessage("Ошибка: " +fil->errorString());
    }
    else
    {
        QTextStream *ts = new QTextStream(fil);
        *ts << "epsilon\tsigma [MPa]\n";
        for (int i = 0;i < *max_index_i;++i)
        {
            *ts << ddd[0][i] << "\t" << ddd[1][i] << "\n";
        }
        fil->close();
        ui->statusBar->showMessage("Файл " + *s + " успешно сохранен");
        QMessageBox::information(this,"","Сохранение прошло успешно");
    }

    ui->lineEdit->setFocus();
}

double MainWindow::q_str_to_double(QString str)
{
    return str.replace(",",".").toDouble();
}

void MainWindow::slotMouseMove(QMouseEvent *event)
{
    double coordX = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    tracer->setGraphKey(coordX);
    ui->statusBar->showMessage("ε: "+QString::number(tracer->position->key())+"\t\tσ: "+QString::number(tracer->position->value()));
    ui->customPlot->replot();
}

MainWindow::~MainWindow()
{
    sett->setValue("SETTING/k",QString::number(q_str_to_double(ui->lineEdit_2->text())));
    sett->setValue("SETTING/a",QString::number(q_str_to_double(ui->lineEdit_3->text())));
    sett->setValue("SETTING/b",QString::number(q_str_to_double(ui->lineEdit_4->text())));
    sett->setValue("SETTING/l0",QString::number(q_str_to_double(ui->lineEdit_5->text())));
    sett->setValue("SETTING/v",QString::number(q_str_to_double(ui->lineEdit_6->text())));

    sett->setValue("SETTING/mw_width",this->width());
    sett->setValue("SETTING/mw_height",this->height());

    for (int i = 0;i < 2;i++)
    {
        delete [] ddd[i];
    }
    delete [] ddd;
    delete max_index_i;

    //delete tracer;

    delete ui;

    delete sett;
}
