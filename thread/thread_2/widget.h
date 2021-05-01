#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mythread.h"

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
    void on_buttonStart_clicked();
    void on_buttonStop_clicked();
    void dealSignal();
    void dealClose();

signals:
    void startThread(); // 启动子线程的信号

private:
    Ui::Widget *ui;
    MyThread *myT;
    QThread *thread;
};

#endif // WIDGET_H
