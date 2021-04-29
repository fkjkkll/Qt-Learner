#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QResizeEvent>
#include <QMoveEvent>
#include <QMouseEvent>
#include <QScreen>


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    this->setStyleSheet("QWidget#widget{background:rgb(66,66,66)}");

    this->setWindowTitle("MyTitleBar");
    connect(ui->titlebar, TitleBar::minWindow, this, dealMinWindow);
    connect(ui->titlebar, TitleBar::maxWindow, this, dealMaxWindow);
    connect(ui->titlebar, TitleBar::unmaxWindow, this, dealUnmaxWindow);
    connect(ui->titlebar, TitleBar::cloWindow, this, dealCloWindow);
    connect(ui->titlebar, TitleBar::movWindow, this, dealMovWindow);

}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::resizeEvent(QResizeEvent *event){
    _oldSize = event->oldSize();
    _newSize = event->size();
}

void MainWidget::moveEvent(QMoveEvent *event){
    _oldPos = event->oldPos();
    _newPos = event->pos();
}

void MainWidget::dealCloWindow(){
    this->close();
}

void MainWidget::dealMaxWindow(){
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();
    this->setGeometry(0,0,screen_width,screen_height);//1920x1030(少50px是底下windows10的菜单栏)
}

void MainWidget::dealUnmaxWindow(){
    this->setGeometry(_oldPos.x(), _oldPos.y(), _oldSize.width(), _oldSize.height());
}

void MainWidget::dealMinWindow(){
    this->showMinimized();
}

void MainWidget::dealMovWindow(QPoint p){
    this->move(p);
}
