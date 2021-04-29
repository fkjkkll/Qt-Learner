#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    int grid_w;
    int grid_h;
    int chess_x;
    int chess_y;
    int gridnum;
    QPainter artist;
    QPen pencil;
    bool startdrawchess;
};

#endif // WIDGET_H
