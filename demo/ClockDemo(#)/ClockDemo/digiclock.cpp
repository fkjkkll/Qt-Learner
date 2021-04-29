#include "digiclock.h"
#include <QTimer>
#include <QTime>
#include <QMouseEvent>
#include <QDebug>

DigiClock::DigiClock(QWidget *parent)
    : QLCDNumber(parent)
{
    QPalette p = palette();
    p.setBrush(QPalette::Window, QBrush(QColor(150,200,50), Qt::SolidPattern));
    p.setBrush(QPalette::WindowText, QBrush(QColor(255,0,0), Qt::SolidPattern));
    setPalette(p);

    setSegmentStyle(Flat);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.6);
    setFrameShape(QFrame::NoFrame);
    resize(300,100);
    setDigitCount(8);                       //设置数字盘显示数字的位数

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));

    timer->start(1000);
    showTime();

    showColon = true;                                 //用于显示冒号
}

void DigiClock::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    if(showColon)
    {
        text[2] = ':';text[5] = ':';
        showColon = false;
    }
    else
    {
        text[2] = ' ';text[5] = ' ';
        showColon = true;
    }
    display(text);
}

void DigiClock::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
    else if(event->button() == Qt::MidButton)
        close();

}
void DigiClock::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}


