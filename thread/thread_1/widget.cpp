#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    myTimer = new QTimer(this);

    connect(myTimer, QTimer::timeout, this, dealNumber);

    // 分配空间，指定父对象
    thread = new MyThread(this);

    connect(thread, MyThread::isDone, this, dealDone);   // 如果是用线程尽量用传统的方式

    connect(this, Widget::destroyed, this, stopThread);  // 线程虽是共享，线程号是固定的，也要记得关闭
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    myTimer->start(100);

    // 启动线程，处理数据
    thread->start();
}

void Widget::dealNumber(){
    static int sec = 0;
    ui->lcdNumber->display(++sec);
}

void Widget::dealDone(){
    qDebug()<<"OVER";
    myTimer->stop();
}

void Widget::stopThread(){
    thread->quit();         // 处理完后才会关闭，但是线程里是死循环不行，所以还是要额外的标志位
    thread->wait();
//    thread->terminate();    // 暴力（强制关闭，操作内存时，可能会丢失数据）
}


