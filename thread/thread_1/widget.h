#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include "mythread.h"   // 线程头文件

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void dealNumber();  // QTimer触发槽函数
    void dealDone();    // 线程结束槽函数
    void stopThread();  // 停止线程槽函数

private:
    Ui::Widget *ui;

    QTimer *myTimer;
    MyThread *thread;
};

#endif // WIDGET_H
