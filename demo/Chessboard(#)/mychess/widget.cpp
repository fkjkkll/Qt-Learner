#include "widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    startdrawchess = false;
    gridnum = 10;

}

Widget::~Widget()
{

}

void Widget::mousePressEvent(QMouseEvent *event){
//    if(event->x()<=grid_w || event->x()>=width()-grid_w || event->y()<=grid_h || event->y()>=height()-grid_h)
//        return;
    if(event->x()/grid_w<1 || event->x()/grid_w>gridnum || event->y()/grid_h<1 || event->y()/grid_h>gridnum)
        return; //better
    startdrawchess = true;
    chess_x = event->x()/grid_w;
    chess_y = event->y()/grid_h;
    update();
}

void Widget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    grid_w = this->width()/(gridnum+2);
    grid_h = this->height()/(gridnum+2);
    artist.begin(this);
    pencil.setWidth(3);
    pencil.setColor(QColor(0,122,222));
    artist.setPen(pencil);
    for(int i=0; i<=gridnum; i++){
        artist.drawLine((i+1)*grid_w, grid_h, (i+1)*grid_w, (gridnum+1)*grid_h);
        artist.drawLine(grid_w, (i+1)*grid_h, (gridnum+1)*grid_w, (i+1)*grid_h);
    }
    if(startdrawchess){
        artist.drawPixmap(chess_x*grid_w, chess_y*grid_h, grid_w, grid_h, QPixmap("://face.png"));
    }
    artist.end();
}
