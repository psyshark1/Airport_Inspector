#include "dialogcalendar.h"
#include "ui_dialogcalendar.h"

DialogCalendar::DialogCalendar(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCalendar)
{
    ui->setupUi(this);
    ui->calendarWidget->setGridVisible(true);
    ui->calendarWidget->setMinimumDate(QDate::fromString(RESTRICT_DATE_FROM,"dd.MM.yyyy"));
    ui->calendarWidget->setMaximumDate(QDate::fromString(RESTRICT_DATE_TO,"dd.MM.yyyy"));
    ui->calendarWidget->setCurrentPage(2017,1);
}

DialogCalendar::~DialogCalendar()
{
    delete ui;
}

void DialogCalendar::on_pb_OK_clicked()
{
    this->hide();
    emit sig_Senddate(ui->calendarWidget->selectedDate());
}

void DialogCalendar::RestrictDates(const QDate& from, const QDate& to)
{
    ui->calendarWidget->setMinimumDate(from);
    ui->calendarWidget->setMaximumDate(to);
}
