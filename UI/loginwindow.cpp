#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHttpMultiPart>

#include <QJsonDocument>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QDir>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pbConnect_clicked()
{
    QString response;
    QString username = ui->leUserName->text();
    QString password = ui->lePassword->text();
    QByteArray read;

    QUrlQuery payload;
    payload.addQueryItem("username", username);
    payload.addQueryItem("password", password);

    QNetworkAccessManager * man = new QNetworkAccessManager(this);

    QNetworkRequest req(QUrl("https://www.digiflixlivestream.com/api/v1/"));
    QString ua("HttpRequestDemo/0.1 (Win64) Qt/5.7");
    QString ct("x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader,QVariant(ua));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    
    connect(man,SIGNAL(finished(QNetworkReply*)),this,SLOT(onfinish(QNetworkReply*)));
    connect(man,SIGNAL(finished(QNetworkReply*)),man,SLOT(deleteLater()));

    man->post(req,payload.toString(QUrl::FullyEncoded).toUtf8());

}

void LoginWindow::onfinish(QNetworkReply *rep)
{
    QByteArray bts = rep->readAll();
    QString str(bts);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(bts);
    QJsonObject root = jsonDoc.object();

    QString url = root["rmtp url"].toString();
    QString key = root["stream key"].toString();
    QString text = url + "\n" + key;

    if(url == "" && key =="")
    {
        if(root["status"] == false)
        {
            QMessageBox::information(this,"sal",root["reason"].toString(),"ok");
        }
        else
        {
            QMessageBox::information(this,"sal",str,"ok");
        }
        return;
    }

    if (QFile::exists("steamInfo.txt"))
    {
        QFile file3 ("steamInfo.txt");
        file3.remove();
    }

    QString filename = "steamInfo.txt";
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
       QTextStream stream(&file);
       stream << url << endl;
       stream << key << endl;
    }

    QMessageBox::information(this,"sal","Connection Established","ok");

    //QApplication::quit();
    this->hide();
}
