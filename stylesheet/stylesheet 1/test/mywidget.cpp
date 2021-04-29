#include "mywidget.h"
#include "ui_mywidget.h"
#include <QtEvents>
#include <QDebug>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    posoffset = new QPoint(0,0);

    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    this->setFixedSize(470,360);//设置窗口为固定大小

    ui->InerLab->setGeometry(0,30,470,360);//设置标题区域大小
    ui->InerLab->setStyleSheet("QLabel{background-color:rgb(255,255,255);}");//设置标题区域颜色

    ui->TitleLab->setGeometry(0,0,470,30);//设置标题区域位置和大小
    ui->TitleLab->setStyleSheet("QLabel{background-color:rgb(53,65,89);border-top-left-radius:8px;border-top-right-radius:8px;}");//设置标题区域颜色


    ui->CloseBtn->setGeometry(432,3,25,25);
    ui->CloseBtn->setIcon(QIcon("://close2.png"));
    ui->CloseBtn->setToolTip(tr("关闭"));


//    this->ui->CloseBtn->setStyleSheet("QPushButton{background-color:transparent;}"
//                                      "QPushButton:hover{background-color:rgb(233,66,66);}"
//                                      );

    this->setStyleSheet(
                "QPushButton#CloseBtn{background-color:transparent;"
                "border-top-left-radius:5px;border-top-right-radius:5px;"
                "border-bottom-left-radius:5px;border-bottom-right-radius:5px;}"
                "QPushButton:hover#CloseBtn{background-color:rgb(233,66,66);}"
                "QPushButton:pressed#CloseBtn{background-color:rgb(53,165,89);}"
    );


    ui->CloseBtn->installEventFilter(this);


}

MyWidget::~MyWidget()
{
    delete ui;
}


void MyWidget::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        *posoffset = event->globalPos() - this->pos();
        event->accept();
    }
}

void MyWidget::mouseMoveEvent(QMouseEvent *event){
    if ((event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - *posoffset);
        event->accept();
    }
}

bool MyWidget::eventFilter(QObject *watched, QEvent *event){
    if(watched == ui->CloseBtn && event->type()==QEvent::MouseMove) //防止点击关闭按钮能给拖动窗口
        return true;
    return QWidget::eventFilter(watched, event);
}

void MyWidget::on_CloseBtn_released()
{
    if(ui->CloseBtn->geometry().contains(this->mapFromGlobal(QCursor::pos())))
        this->close();
}
