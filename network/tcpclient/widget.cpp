#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#define cout qDebug() << "[" << __FILE__ <<":" << __LINE__ << "]"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Client");

    // 分配控件，指定父对象
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, QTcpSocket::connected,
            [=](){
        connected = true;
        ui->textEditRead->append("成功和服务器建立好连接");
    });

    connect(tcpSocket, QTcpSocket::readyRead,
            [=](){
        // 获取对方发送的内容
        QByteArray array = tcpSocket->readAll();

        // 追加到编辑区中
        ui->textEditRead->append(array);
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_buttonConnect_clicked()
{
    if(connected)
        return;

    // 获取服务器ip和端口
    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();

    // 主动和服务器建立连接
    tcpSocket->connectToHost(ip, port);
}

void Widget::on_buttonSend_clicked()
{
    if(!connected)
        return;

    // 获取编辑框内容
    QString str = ui->textEditWrite->toPlainText();

    // 发送数据
    tcpSocket->write(str.toUtf8());
    ui->textEditWrite->clear();
}

void Widget::on_buttonClose_clicked()
{
    if(!connected)
        return;

    // 主动和对方断开连接
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    connected = false;
}




