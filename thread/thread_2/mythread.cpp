#include "mythread.h"
#include <QThread>
#include <QDebug>

MyThread::MyThread(QObject *parent) : QObject(parent)
{
    _stop = false;
}

void MyThread::myTimerout(){
    while(!_stop){
        QThread::msleep(10);
        emit mySignal();
        qDebug() << "子线程号：" <<QThread::currentThread();
    }
}

void MyThread::setFlag(bool flag){
    _stop = flag;
}
