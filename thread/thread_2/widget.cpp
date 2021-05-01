#include "widget.h"
#include "ui_widget.h"
#include <QThread>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 动态分配控件，不能指定父对象
    myT = new MyThread;

    // 创建子线程
    thread = new QThread(this);

    // 把自定义线程加入到子线程中
    myT->moveToThread(thread);

    connect(myT, MyThread::mySignal, this, dealSignal);
    connect(this, startThread, myT, MyThread::myTimerout);
    connect(this, Widget::destroyed, this, dealClose);

    qDebug() << "主线程号：" <<QThread::currentThread();

    // 线程处理函数内部，不允许操作图形界面
    // 比如QMessageBox::aboutQt(nullptr)放到线程处理函数里会报错

    // connect()第五个参数的作用，连接方式：默认、队列、直接
    // 只有在多线程才有意义
    // 默认的时候，如果是多线程默认用队列
    // 如果是单线程，默认使用直接方式
    // 如果接收者所在线程和发送信号的线程一样则使用直接，否则使用队列
}

Widget::~Widget()
{
    delete ui;
    delete myT; // 因为不能指定父对象，所以需要手动释放
}

void Widget::dealSignal(){
    static int sec = 0;
    ui->lcdNumber->display(++sec);
}

void Widget::on_buttonStart_clicked(){

    if(thread->isRunning() == true){
        return;
    }

    // 启动线程，但是没有启动线程处理函数
    thread->start();
    myT->setFlag(false);

    // 不能直接调用线程处理函数，否则是在同一线程中
    // 只能通过 signal-slot 方式调用
    emit startThread();
}

void Widget::on_buttonStop_clicked(){
    if(thread->isRunning() == false){
        return;
    }
    myT->setFlag(true);
    thread->quit();
    thread->wait();
}

void Widget::dealClose(){
    on_buttonStop_clicked();
}
