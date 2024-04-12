#include "graphwindow.h"
#include "ui_graphwindow.h"

GraphWindow::GraphWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);
}

GraphWindow::~GraphWindow()
{
    delete ui;
}

void GraphWindow::on_comboBox_currentIndexChanged(int index)
{
    emit sig_IndexMonth(index);
}

void GraphWindow::on_pb_close_clicked()
{
    this->hide();
}
