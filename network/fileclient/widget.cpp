#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDebug>
#define cout qDebug() << "[" << __FILE__ <<":" << __LINE__ << "]"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);

    tcpSocket = new QTcpSocket(this);
    isStart = true;
    connect(tcpSocket, QTcpSocket::readyRead, [=](){
        // 取出接收的内容
        QByteArray buf = tcpSocket->readAll();
        if(true == isStart){
            //接收头
            isStart = false;
            ui->progressBar->setValue(0);
            // 解析头部信息并初始化 buf = "hello##1024"
            fileName = QString(buf).section("##", 0, 0);
            fileSize = QString(buf).section("##", 1, 1).toInt();
            receivedFileSize = 0;
            ui->textBrowser->append(QString("正在接收文件[%1(%2M)]").arg(fileName).arg(fileSize>>20));
            // 打开文件
            file.setFileName(fileName);
            if(!file.open(QIODevice::WriteOnly)){
                cout<<"客户端打开文件出错";
            }
        }
        else{
            // 接收文件内容
            qint64 len = file.write(buf);
            receivedFileSize += len;
            ui->progressBar->setValue(100 * receivedFileSize/fileSize);
            if(receivedFileSize == fileSize){
                ui->textBrowser->append(QString("接收文件完成[%1(%2M)]").arg(fileName).arg(fileSize>>20));

                file.close();
                tcpSocket->disconnectFromHost();
                tcpSocket->close();
                isStart = true;
            }

        }
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_buttonConnect_clicked()
{
    // 获取服务器的ip和端口
    QString ip = ui->lineEditIP->text();
    quint16 port = ui->lineEditPort->text().toInt();
    tcpSocket->connectToHost(QHostAddress(ip), port);
}







