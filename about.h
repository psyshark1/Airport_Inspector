#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();
private slots:
    void on_pushButton_clicked();

private:
    Ui::AboutWindow *ui;
};

#endif // ABOUT_H
