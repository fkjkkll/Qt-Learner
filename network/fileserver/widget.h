#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>   // 监听套接字
#include <QTcpSocket>   // 通信套接字
#include <QTimer>
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

    void sendData();    // 发送文件数据

private slots:
    void on_buttonFile_clicked();

    void on_buttonSend_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *tcpServer;  // 监听套接字
    QTcpSocket *tcpSocket;  // 通信套接字
    QFile file;
    QString fileName;
    qint64 fileSize;
    qint64 receivedFileSize;


};

#endif // WIDGET_H
