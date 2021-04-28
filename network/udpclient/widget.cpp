#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("服务器端口8888");

    // 分配控件，指定父对象
    udpSocket = new QUdpSocket(this);
    udpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 1); //设置Socket为组播模式

    // 绑定
    udpSocket->bind(8888);

//    // 绑定：组播模式
//    udpSocket->bind(QHostAddress::AnyIPv4, 8888, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
//    udpSocket->joinMulticastGroup(QHostAddress("224.0.0.211"));


    // 当对方成功发送数据过来
    // 自动触发 readyRead()
    connect(udpSocket, QUdpSocket::readyRead, this, dealMsg);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::dealMsg(){
    // 读取对方发送的内容
    char buf[1024] = {0};
    QHostAddress cliAddr; // 对方地址
    quint16 cliPort; // 对方端口
    qint64 len = udpSocket->readDatagram(buf, sizeof(buf), &cliAddr, &cliPort);
    if(len>0){
        // 格式化 [192.168.2.2:8888] context
        QString str = QString("[%1:%2] %3").arg(cliAddr.toString()).arg(cliPort).arg(buf);

        // 设置编辑区内容
        ui->textEditRead->append(str);
    }

}

void Widget::on_buttonSend_clicked()
{
    // 先获取对方的IP和端口
    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();

    // 获取编辑区的内容
    QString str = ui->textEditWrite->toPlainText();

    // 给指定的IP发送数据
    udpSocket->writeDatagram(str.toUtf8(), QHostAddress(ip), port);

    ui->textEditWrite->clear();
}
