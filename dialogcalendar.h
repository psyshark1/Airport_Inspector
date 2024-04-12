#ifndef DIALOGCALENDAR_H
#define DIALOGCALENDAR_H

#include <QDialog>
#include "database.h"

namespace Ui {
class DialogCalendar;
}

class DialogCalendar : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCalendar(QWidget *parent = nullptr);
    ~DialogCalendar();
    void RestrictDates(const QDate& from, const QDate& to);

private slots:
    void on_pb_OK_clicked();

signals:
    void sig_Senddate(const QDate &date);
private:
    Ui::DialogCalendar *ui;
};

#endif // DIALOGCALENDAR_H
