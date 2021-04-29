#include "widget.h"
#include "ui_widget.h"

#include <QMouseEvent>
#include <QToolButton>
#include <QPixmap>
#include <QEvent>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    pos_offset = new QPoint(0,0);

    //去掉窗口边框
    setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明

    //获取最小化、关闭按钮图标
    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    QPixmap maxPix = style()->standardPixmap(QStyle::SP_TitleBarMaxButton);

    //设置最小化、关闭按钮图标
    ui->minButton->setIcon(minPix);
    ui->closeButton->setIcon(closePix);
    ui->maxButton->setIcon(maxPix);

    //设置鼠标移至按钮上的提示信息
    ui->minButton->setToolTip(tr("最小化"));
    ui->maxButton->setToolTip(tr("最大化"));
    ui->closeButton->setToolTip(tr("关闭"));

    //设置最小化、关闭按钮的样式
    this->setStyleSheet(
                "QToolButton{background-color:transparent;border-top-left-radius:8px;border-top-right-radius:8px;"
                "border-bottom-left-radius:8px;border-bottom-right-radius:8px;}"
                "QToolButton:hover#minButton, QToolButton:hover#maxButton{background-color:rgb(116,116,116);}"
                "QToolButton:hover#closeButton, maxButton{background-color:rgb(229,61,61);}"
                "QWidget#mainwidget{background-color:rgb(200,200,200);border-bottom-left-radius:8px;border-bottom-right-radius:8px;}"
                "QWidget#toolWidget{background-color:rgb(53,65,89);border-top-left-radius:8px;border-top-right-radius:8px;}"
                );

    this->ui->closeButton->setFocusPolicy(Qt::NoFocus);
    this->ui->maxButton->setFocusPolicy(Qt::NoFocus);
    this->ui->minButton->setFocusPolicy(Qt::NoFocus);


}



Widget::~Widget(){
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        *pos_offset = event->globalPos() - this->pos();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        move(event->globalPos() - *pos_offset);
    }
}

void Widget::on_closeButton_clicked(){
    this->close();
}

void Widget::on_maxButton_clicked(){
    if(!this->isMaximized()){
        QPixmap mulPix = style()->standardPixmap(QStyle::SP_TitleBarNormalButton);
        ui->maxButton->setIcon(mulPix);
        ui->maxButton->setToolTip(tr("取消最大化"));
        this->showMaximized();
    }
    else{
        QPixmap maxPix = style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
        ui->maxButton->setIcon(maxPix);
        ui->maxButton->setToolTip(tr("最大化"));
        this->showNormal();
    }
}

void Widget::on_minButton_clicked(){
    this->showMinimized();
}
