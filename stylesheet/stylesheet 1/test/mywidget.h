#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    ~MyWidget();

protected:
    bool eventFilter(QObject *watched, QEvent *event);



private slots:
    void on_CloseBtn_released();

private:
    Ui::MyWidget *ui;
    QPoint* posoffset;
};

#endif // MYWIDGET_H
