#include "about.h"
#include "ui_about.h"

AboutWindow::AboutWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::on_pushButton_clicked()
{
    this->close();
}

