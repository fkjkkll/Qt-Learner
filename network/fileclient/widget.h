#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>

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
    void on_buttonConnect_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket* tcpSocket;
    QFile file;
    QString fileName;
    qint64 fileSize;
    qint64 receivedFileSize;
    bool isStart;
};

#endif // WIDGET_H
