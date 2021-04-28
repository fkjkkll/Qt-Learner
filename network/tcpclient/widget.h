#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>

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

    void on_buttonSend_clicked();

    void on_buttonClose_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;
    bool connected = false;
};

#endif // WIDGET_H
