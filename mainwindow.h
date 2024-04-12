#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include <QTableWidgetItem>
#include <QTimer>

#include "graphwindow.h"
#include "dialogcalendar.h"
#include "about.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer* qtimer{nullptr};
    Database* database{nullptr};
    QMessageBox* msg{nullptr};
    GraphWindow* gw{nullptr};
    AboutWindow* abw{nullptr};
    Graphic* graphic{nullptr};
    Barchart* barChart{nullptr};
    QMap<QString, QString>* AirPortLists{nullptr};
    QList<QPair<QDate, uint16_t>>* AllDaysFlightYear{nullptr};
    DialogCalendar* dc{nullptr};
    QDate* shedule{nullptr};
    QDate* from{nullptr};
    QDate* to{nullptr};
    QList<QDate>* RestrictMonthDates{nullptr};
    QTableWidgetItem* airport{nullptr};
    QVector<double>* X{nullptr};
    QVector<double>* Y{nullptr};
    unsigned char flagDates{0b00000000};
    void ReconnectToDatabase();
    void GetTableFlightDirection(QSqlQuery& q, QString airportDirection);
    void SetPeriodFlights(const QDate& from,const QDate& to);
signals:
    void sig_SendRestrictDates(const QDate& from, const QDate& to);
public slots:
    void ReceiveStatusConnectionToDB(const bool& status);
    void ReceiveStatusRequestToDB(QSqlError err, const int& reqType);
    void ReceiveDate(const QDate& date);
    void ReceiveIdxMonth(const int& index);
private slots:
    void on_pb_setDateTo_clicked();
    void on_pb_setDateFrom_clicked();
    void on_pb_setDateShedule_clicked();
    void on_pb_GetDirectionFlight_clicked();
    void on_actionquit_triggered();
    void on_pb_getStats_clicked();
    void on_cb_airportList_currentIndexChanged(int index);
    void on_cb_flightDirection_currentIndexChanged(int index);
    void on_action_triggered();
};
#endif // MAINWINDOW_H
