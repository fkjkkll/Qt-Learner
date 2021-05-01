#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>

class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);

    // 线程处理函数
    void myTimerout();
    void setFlag(bool flag = true);

signals:
    void mySignal();

public slots:

private:
    bool _stop;
};

#endif // MYTHREAD_H
