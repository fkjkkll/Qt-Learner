#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
#define cout qDebug() << "[" << __FILE__ <<":" << __LINE__ << "]"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("Server");
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any, 8888);//QHostAddress::AnyIPv4    QHostAddress("192.168.137.1")
    connect(tcpServer, QTcpServer::newConnection,
            [=](){
        //取出建立好连接的套接字
        tcpSocket = tcpServer->nextPendingConnection();

        //获取对方IP和端口
        QString ip = tcpSocket->peerAddress().toString();
        qint16 port = tcpSocket->peerPort();
        QString temp = QString("[%1:%2]:成功连接").arg(ip).arg(port);
        ui->textEditRead->append(temp);

        connect(tcpSocket, QTcpSocket::readyRead,
                [=](){
            //从通信套接字去除内容
            QByteArray array = tcpSocket->readAll();
            ui->textEditRead->append(array);
        });
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_buttonSend_clicked()
{
    if(!tcpSocket)
        return;
    //获取编辑区内容
    QString str = ui->textEditWrite->toPlainText();

    //线
    tcpSocket->write(str.toUtf8());
    ui->textEditWrite->clear();
}

void Widget::on_buttonClose_clicked()
{
    if(!tcpSocket)
        return;
    //主动和客户端断开连接
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    delete tcpSocket;
    tcpSocket = nullptr;
}






