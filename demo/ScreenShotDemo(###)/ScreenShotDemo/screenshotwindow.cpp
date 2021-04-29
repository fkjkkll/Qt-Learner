#include "screenshotwindow.h"
#include "ui_screenshotwindow.h"
#include <QDesktopWidget>
#include <QDebug>
#define cout qDebug() << "[" << __FILE__ <<":" << __LINE__ << "]"

ScreenShotWindow::ScreenShotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ScreenShotWindow)
{
    pos_offset = new QPoint(0, 0);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    fullWidget = new FullScreenWidget();
    //QDesktopWidget desktop;
    //fullWidget->setGeometry(desktop.screenGeometry(1));
    connect(fullWidget, fullWidget->mainwinhide, this, forhide);
    connect(fullWidget, fullWidget->mainwinshow, this, forshow);
    connect(fullWidget, fullWidget->mainwinclose, this, close);
    connect(ui->btscreenshot, QPushButton::clicked, this, screenshot);
    connect(this,SIGNAL(setPixmap(QPixmap)),fullWidget,SLOT(loadBackgroundPixmap(QPixmap)));
    ui->btscreenshot->installEventFilter(this);
}

ScreenShotWindow::~ScreenShotWindow(){
    delete ui;
}

void ScreenShotWindow::screenshot(){
    QPixmap pixmap = fullWidget->getFullScreenPixmap();
    fullWidget->show();

    //发送信号，使用当前的屏幕的图片作背景图片
    emit setPixmap(pixmap);
}

void ScreenShotWindow::forhide(){
    this->hide();
}

void ScreenShotWindow::forshow(){
    this->show();
}

void ScreenShotWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        *pos_offset = event->globalPos() - this->pos();
    }
    else
        close();
}

void ScreenShotWindow::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        //move(event->globalPos() - event->pos());错误:变量-变量 = 0
        move(event->globalPos() - (*pos_offset));
    }
}

void ScreenShotWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape)
        close();

}

bool ScreenShotWindow::eventFilter(QObject *watched, QEvent *event){
    if(watched==ui->btscreenshot && event->type() == QEvent::MouseMove)
        return true;
    return QMainWindow::eventFilter(watched, event);
}
