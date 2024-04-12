#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui_graphwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    dc(new DialogCalendar(this)),
    gw(new GraphWindow(this)),
    AirPortLists(new QMap<QString, QString>),
    AllDaysFlightYear(new QList<QPair<QDate, uint16_t>>),
    RestrictMonthDates(new QList<QDate>),
    shedule(new QDate{2017,1,1}),
    from(new QDate{2017,1,1}),
    to(new QDate{2017,1,1})
{
    ui->setupUi(this);

    ui->lb_status->setStyleSheet("color:red");
    airport = new QTableWidgetItem("Аэропорт");
    ui->tw_result->setColumnCount(3);
    ui->tw_result->setHorizontalHeaderItem(0,new QTableWidgetItem("Номер рейса"));
    ui->tw_result->setHorizontalHeaderItem(1,new QTableWidgetItem("Время вылета"));
    ui->tw_result->setHorizontalHeaderItem(2,airport);

    RestrictMonthDates->append(QDate{2017,1,1});
    RestrictMonthDates->append(QDate{2017,1,31});
    database = new Database(this);
    msg = new QMessageBox(this);
    msg->setWindowTitle("Инспектор аэропортов");
    abw = new AboutWindow(this);
    qtimer = new QTimer(this);
    qtimer->setInterval(CONN_INTERVAL);
    graphic = new Graphic(gw->ui->qcp_month, 1, gw);
    barChart = new Barchart(gw->ui->qcp_year, 1, gw);
    X = new QVector<double>;
    for (int i = 1; i < 32; ++i){X->append(static_cast<double>(i));}

    connect(dc, &DialogCalendar::sig_Senddate, this, &MainWindow::ReceiveDate);
    connect(gw, &GraphWindow::sig_IndexMonth, this, &MainWindow::ReceiveIdxMonth);
    connect(database, &Database::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(database, &Database::sig_SendStatusRequest, this, &MainWindow::ReceiveStatusRequestToDB);
    connect(this, &MainWindow::sig_SendRestrictDates, dc, &DialogCalendar::RestrictDates);
    connect(qtimer, &QTimer::timeout, this, [&]{QtConcurrent::run([&]{ui->statusbar->setStyleSheet("color:white");ui->statusbar->showMessage(dbInfo.Connection);database->ConnectToDataBase();});});

    QtConcurrent::run([&]{ui->statusbar->setStyleSheet("color:white");ui->statusbar->showMessage(dbInfo.Connection);database->ConnectToDataBase();});
}

MainWindow::~MainWindow()
{
    delete shedule;delete from;delete to; delete airport; delete AirPortLists; delete ui; delete X; delete Y;
}

void MainWindow::ReceiveStatusConnectionToDB(const bool& status)
{
    if(status)
    {
        if(qtimer->isActive()){qtimer->stop();}
        ui->lb_status->setStyleSheet("color:green");
        ui->lb_status->setText(dbInfo.Connected);

        if(!((flagDates >> 3) & 1))
        {
            ui->statusbar->setStyleSheet("color:white");
            ui->statusbar->showMessage(dbInfo.ExecuteSQLrequest);
            QtConcurrent::run([&]{database->RequestToDB(requests.GetAirportList, requestType::requestAirportList);});
        }
        else
        {
            ui->pb_getStats->setEnabled(true);
            ui->pb_GetDirectionFlight->setEnabled(true);
            ui->cb_airportList->setEnabled(true);
        }
    }
    else
    {
        ui->lb_status->setStyleSheet("color:red");
        ui->lb_status->setText(dbInfo.Disconnected);
        ui->statusbar->setStyleSheet("color:red");
        ui->statusbar->showMessage(dbInfo.ErrorConnection);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(database->GetLastError().text());
        msg->exec();
        if(!qtimer->isActive()){qtimer->start();}
    }
}

void MainWindow::ReceiveStatusRequestToDB(QSqlError err, const int& reqType)
{
    if(err.type() != QSqlError::NoError)
    {
        ui->statusbar->setStyleSheet("color:red");
        ui->statusbar->showMessage(dbInfo.ErrorSQLrequest);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(err.text());
        msg->exec();
        ui->pb_GetDirectionFlight->setEnabled(true);
        return;
    }
    ui->statusbar->setStyleSheet("color:green");
    ui->statusbar->showMessage(dbInfo.OK);

    QSqlQuery& q = database->GetQuery();
    switch (reqType)
    {
    case requestType::requestAirportList:
        {
            while (q.next())
            {
                AirPortLists->insert(q.value(0).toString(), q.value(1).toString());
                ui->cb_airportList->addItem(q.value(0).toString());
            }

            flagDates = (flagDates | (1 << 3));
            //if(((flagDates >> 3) & 1))
            //{
                ui->cb_airportList->setEnabled(true);
                ui->pb_GetDirectionFlight->setEnabled(true);
                ui->pb_getStats->setEnabled(true);
            //}
            break;
        }
    case requestType::requestAllStatsByDays:
        {
            QPair<QDate, uint16_t> tmp;
            AllDaysFlightYear->clear();
            while (q.next())
            {
                tmp.first = q.value(1).toDate(); tmp.second = q.value(0).toInt();
                AllDaysFlightYear->append(tmp);
            }
            if(Y != nullptr){Y->clear();}else{Y = new QVector<double>;}
            int curmonth = AllDaysFlightYear->cbegin()->first.month();
            double msumm{0.0};
            for (auto i = AllDaysFlightYear->cbegin(); i != AllDaysFlightYear->cend(); ++i)
            {
                if (curmonth == i->first.month())
                {
                    msumm += static_cast<double>(i->second);
                }
                else
                {
                    Y->append(msumm);
                    msumm = static_cast<double>(i->second);
                }
                curmonth = i->first.month();
            }
            Y->append(msumm);

            barChart->RecieveData(*Y,gw->ui->qcp_year,0);
            SetPeriodFlights(QDate{2017,1,1},QDate{2017,1,31});
            flagDates = flagDates | (1 << 4);
            gw->setWindowTitle("Загруженность аэропорта " + ui->cb_airportList->currentText());
            gw->show();

            ui->cb_airportList->setEnabled(true);
            ui->pb_GetDirectionFlight->setEnabled(true);
            ui->pb_getStats->setEnabled(true);
            break;
        }
    case requestType::requestDepartPlanes:
        {
            GetTableFlightDirection(q,"Аэропорт назначения");
            ui->cb_airportList->setEnabled(true);
            ui->pb_GetDirectionFlight->setEnabled(true);
            ui->pb_getStats->setEnabled(true);
            break;
        }
    case requestType::requestArrivalPlanes:
        {
            GetTableFlightDirection(q,"Аэропорт отправления");
            ui->cb_airportList->setEnabled(true);
            ui->pb_GetDirectionFlight->setEnabled(true);
            ui->pb_getStats->setEnabled(true);
            break;
        }
    }
}

void MainWindow::GetTableFlightDirection(QSqlQuery& q, QString airportDirection)
{
    uint16_t row{0};
    ui->tw_result->clearContents();
    ui->tw_result->setRowCount(q.size());
    airport->setText(airportDirection);

    while (q.next())
    {
        ui->tw_result->setItem(row,0,new QTableWidgetItem(q.value(0).toString()));
        ui->tw_result->setItem(row,1,new QTableWidgetItem(q.value(1).toString()));
        ui->tw_result->setItem(row,2,new QTableWidgetItem(q.value(2).toString()));
        ++row;
    }
}

void MainWindow::ReceiveDate(const QDate &date)
{
    if((flagDates >> 0) & 1)
    {
        ui->le_dateSchedule->setText(date.toString("dd.MM.yyyy"));

        *shedule = date;
        RestrictMonthDates->begin()->setDate(date.year(), date.month(), 1);
        RestrictMonthDates->rbegin()->setDate(date.year(), date.month(), date.daysInMonth());
        ui->le_dateFrom->setText(from->toString("dd.MM.yyyy"));
        ui->le_dateTo->setText(to->toString("dd.MM.yyyy"));
        return;
    }
    if((flagDates >> 1) & 1)
    {
        ui->le_dateFrom->setText(date.toString("dd.MM.yyyy"));
        *from = date;
    }
    if((flagDates >> 2) & 1)
    {
        ui->le_dateTo->setText(date.toString("dd.MM.yyyy"));
        *to = date;
    }
}

void MainWindow::on_pb_setDateTo_clicked()
{
    flagDates = (flagDates | (1 << 2)) & (~(1 << 0)) & (~(1 << 1)) | (1 << 5);
    emit sig_SendRestrictDates(*RestrictMonthDates->cbegin(), *RestrictMonthDates->crbegin());
    dc->show();
    ui->statusbar->setStyleSheet("color:yellow");
    ui->statusbar->showMessage(dbInfo.FlightsByPeriod);
}

void MainWindow::on_pb_setDateFrom_clicked()
{
    flagDates = (flagDates | (1 << 1)) & (~(1 << 0)) & (~(1 << 2)) | (1 << 5);
    emit sig_SendRestrictDates(*RestrictMonthDates->cbegin(), *RestrictMonthDates->crbegin());
    dc->show();
    ui->statusbar->setStyleSheet("color:yellow");
    ui->statusbar->showMessage(dbInfo.FlightsByPeriod);
}

void MainWindow::on_pb_setDateShedule_clicked()
{
    flagDates = (flagDates | (1 << 0)) & (~(1 << 1)) & (~(1 << 2)) | (1 << 5);
    emit sig_SendRestrictDates(std::move(QDate::fromString(RESTRICT_DATE_FROM,"dd.MM.yyyy")), std::move(QDate::fromString(RESTRICT_DATE_TO,"dd.MM.yyyy")));
    dc->show();
    ui->statusbar->setStyleSheet("color:yellow");
    ui->statusbar->showMessage(dbInfo.FlightsByDate);
}

void MainWindow::ReconnectToDatabase()
{
    ui->lb_status->setStyleSheet("color:red");
    ui->lb_status->setText(dbInfo.Disconnected);
    qtimer->start();
    ui->statusbar->setStyleSheet("color:red");
    ui->statusbar->showMessage(dbInfo.TerminateConnection);
    ui->cb_airportList->setEnabled(false);
    ui->pb_GetDirectionFlight->setEnabled(false);
    ui->pb_getStats->setEnabled(false);
}

void MainWindow::on_pb_GetDirectionFlight_clicked()
{
    if(!database->sqlbindvals->empty())
    {
        if((*database->sqlbindvals->cbegin() == AirPortLists->value(ui->cb_airportList->currentText())) && !((flagDates >> 5) & 1)){return;}
    }
    ui->cb_airportList->setEnabled(false);
    ui->pb_getStats->setEnabled(false);
    ui->pb_GetDirectionFlight->setEnabled(false);
    flagDates = flagDates & (~(1 << 5));
    database->sqlbindvals->clear();
    database->sqlbindvals->append(AirPortLists->value(ui->cb_airportList->currentText()));

    if((flagDates >> 0) & 1)
    {
        database->sqlbindvals->append(shedule->toString("yyyy-MM-dd"));
    }
    else
    {
        if(*from > *to)
        {
            ui->statusbar->setStyleSheet("color:red");
            ui->statusbar->showMessage(dbInfo.WrongPeriod);
            msg->setIcon(QMessageBox::Critical);
            msg->setText(dbInfo.WrongPeriod);
            msg->exec();
            ui->pb_GetDirectionFlight->setEnabled(true);
            return;
        }
        database->sqlbindvals->append(from->toString("yyyy-MM-dd"));
        database->sqlbindvals->append(to->toString("yyyy-MM-dd"));
    }

    if(!database->GetConnectionStatus()){ReconnectToDatabase();return;}

    ui->statusbar->setStyleSheet("color:white");
    ui->statusbar->showMessage(dbInfo.ExecuteSQLrequest);

    (ui->cb_flightDirection->currentText() == "Вылет") ?
        ((flagDates >> 0) & 1) ?
            QtConcurrent::run([&]{;database->RequestToDB(requests.GetDepartPlanesDay, requestType::requestDepartPlanes);})
        :
            QtConcurrent::run([&]{;database->RequestToDB(requests.GetDepartPlanesPeriod, requestType::requestDepartPlanes);})
    :
        ((flagDates >> 0) & 1) ?
            QtConcurrent::run([&]{;database->RequestToDB(requests.GetArrivalPlanesDay, requestType::requestArrivalPlanes);})
        :
            QtConcurrent::run([&]{;database->RequestToDB(requests.GetArrivalPlanesPeriod, requestType::requestArrivalPlanes);});
}

void MainWindow::ReceiveIdxMonth(const int& index)
{
    switch (index)
    {
        case 0:
        {
            SetPeriodFlights(QDate{2017,index+1,1},QDate{2017,index+1,31});
            break;
        }
        case 1:
        {
            SetPeriodFlights(QDate{2017,index+1,1},QDate{2017,index+1,28});
            break;
        }
        case 2:
        {
            SetPeriodFlights(QDate{2017,index+1,1},QDate{2017,index+1,31});
            break;
        }
        case 3:
        {
            SetPeriodFlights(QDate{2017,index+1,1},QDate{2017,index+1,30});
            break;
        }
        case 4:
        {
            SetPeriodFlights(QDate{2017,index+1,1},QDate{2017,index+1,31});
            break;
        }
        case 5:
        {
            SetPeriodFlights(QDate{2017,index+1,1},QDate{2017,index+1,30});
            break;
        }
        case 6:
        {
            SetPeriodFlights(QDate{2017,index+1,1},QDate{2017,index+1,31});
            break;
        }
        case 7:
        {
            SetPeriodFlights(QDate{2017,index+1,1},QDate{2017,index+1,31});
            break;
        }
        case 8:
        {
            SetPeriodFlights(QDate{2016,index+1,1},QDate{2016,index+1,30});
            break;
        }
        case 9:
        {
            SetPeriodFlights(QDate{2016,index+1,1},QDate{2016,index+1,31});
            break;
        }
        case 10:
        {
            SetPeriodFlights(QDate{2016,index+1,1},QDate{2016,index+1,30});
            break;
        }
        case 11:
        {
            SetPeriodFlights(QDate{2016,index+1,1},QDate{2016,index+1,31});
            break;
        }
    }
}

inline void MainWindow::SetPeriodFlights(const QDate& from,const QDate& to)
{
    Y->clear();
    for (int i = 0; i < AllDaysFlightYear->size(); ++i)
    {
        if (AllDaysFlightYear->value(i).first >= from){Y->append(static_cast<double>(AllDaysFlightYear->value(i).second));}
        if (AllDaysFlightYear->value(i).first > to){break;}
    }
    graphic->RecieveData(*X,*Y,gw->ui->qcp_month,0);
}

void MainWindow::on_actionquit_triggered()
{
    this->close();
}

void MainWindow::on_pb_getStats_clicked()
{
    if(((flagDates >> 4) & 1))
    {
        gw->show();
        return;
    }

    ui->cb_airportList->setEnabled(false);
    ui->pb_GetDirectionFlight->setEnabled(false);
    ui->pb_getStats->setEnabled(false);
    if(!database->GetConnectionStatus()){ReconnectToDatabase();}
    database->sqlbindvals->clear();
    database->sqlbindvals->append(AirPortLists->value(ui->cb_airportList->currentText()));
    database->sqlbindvals->append(AirPortLists->value(ui->cb_airportList->currentText()));
    ui->statusbar->setStyleSheet("color:white");
    ui->statusbar->showMessage(dbInfo.ExecuteSQLrequest);
    QtConcurrent::run([&]{database->RequestToDB(requests.AllStatsByDays, requestType::requestAllStatsByDays);});
}

void MainWindow::on_cb_airportList_currentIndexChanged(int index)
{
    flagDates = flagDates & (~(1 << 4));
}

void MainWindow::on_cb_flightDirection_currentIndexChanged(int index)
{
    flagDates = flagDates | (1 << 5);
}

void MainWindow::on_action_triggered()
{
    abw->exec();
}

