#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QtNetwork/QNetworkReply>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_pbConnect_clicked();
    void onfinish(QNetworkReply *rep);

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
