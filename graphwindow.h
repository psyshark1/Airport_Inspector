#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QDialog>
#include "graphic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; class GraphWindow;}
QT_END_NAMESPACE

class GraphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();
    Ui::GraphWindow *ui;

signals:
    void sig_IndexMonth(const int& index);

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_pb_close_clicked();
};

#endif // GRAPHWINDOW_H
