#include "mythread.h"

MyThread::MyThread(QObject *parent) : QThread(parent)
{

}


void MyThread::run(){
    // 很复杂的数据处理需要耗时5s
    sleep(5);

    emit isDone();
}
