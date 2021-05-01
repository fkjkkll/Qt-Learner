#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QMouseEvent>
#include <QScreen>
#include <QDebug>
#define cout qDebug() << "[" << __FILE__ <<":" << __LINE__ << "]"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->setMinimumSize(200, 100);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint |
                         Qt::WindowMinMaxButtonsHint);
    this->setStyleSheet("QWidget#widget{background:rgb(66,66,66)}");
    this->setWindowTitle("MyTitleBar");
    connect(ui->titlebar, TitleBar::minWindow, this, dealMinWindow);
    connect(ui->titlebar, TitleBar::maxWindow, this, dealMaxWindow);
    connect(ui->titlebar, TitleBar::unmaxWindow, this, dealUnmaxWindow);
    connect(ui->titlebar, TitleBar::cloWindow, this, dealCloWindow);
    connect(ui->titlebar, TitleBar::movWindow, this, dealMovWindow);
    connect(ui->titlebar, TitleBar::recordOldPos, [=](){_oldPos = this->geometry().topLeft();});
    this->installEventFilter(this);
    this->setMouseTracking(true);
    ui->widget->setMouseTracking(true); // 要响应的控件上面有其他子控件，一并设置才有效果
    cursorState = normal;
    setCursor(Qt::ArrowCursor);
}

MainWidget::~MainWidget(){
    delete ui;
}

void MainWidget::mousePressEvent(QMouseEvent *event){   // 鼠标按下事件
    Q_UNUSED(event);
    if(!ui->titlebar->isMaxWindow()){
        _oldSize = this->size();
    }
    if(cursorState != normal){
        _oldPos = this->pos();
        isDraging = true;
    }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event){
    if(!ui->titlebar->isMaxWindow() && !isDraging){
        detectEdge(event->pos());
    }
    if(event->buttons() & Qt::LeftButton && cursorState != normal){
        int offset_top_y = event->globalPos().y() - _oldPos.y();
        int offset_left_x = event->globalPos().x() - _oldPos.x();
        int offset_bottom_y = event->globalPos().y() - _oldPos.y() - _oldSize.height();
        int offset_right_x = event->globalPos().x() - _oldPos.x() - _oldSize.width();
        int x = _oldPos.x();
        int y = _oldPos.y();
        int w = _oldSize.width();
        int h = _oldSize.height();
        switch(cursorState){
        case leftTop:
                x += offset_left_x;
                y += offset_top_y;
                w -= offset_left_x;
                h -= offset_top_y;
                if(w < this->minimumSize().width()){
                    x += w - this->minimumSize().width();
                    w = this->minimumSize().width();
                }
                if(h < this->minimumSize().height()){
                    y += h - this->minimumSize().height();
                    h = this->minimumSize().height();
                }
                break;
        case top:
                y += offset_top_y;
                h -= offset_top_y;
                if(h < this->minimumSize().height()){
                    y += h - this->minimumSize().height();
                    h = this->minimumSize().height();
                }
                break;
        case rightTop:
                y += offset_top_y;
                w += offset_right_x;
                h -= offset_top_y;
                if(w < this->minimumSize().width()){
                    w = this->minimumSize().width();
                }
                if(h < this->minimumSize().height()){
                    y += h - this->minimumSize().height();
                    h = this->minimumSize().height();
                }
                break;
        case right:
                w += offset_right_x;
                break;
        case rightBottom:
                w += offset_right_x;
                h += offset_bottom_y;
                break;
        case bottom:
                h += offset_bottom_y;
                break;
        case leftBottom:
                x += offset_left_x;
                w -= offset_left_x;
                h += offset_bottom_y;
                if(w < this->minimumSize().width()){
                    x += w - this->minimumSize().width();
                    w = this->minimumSize().width();
                }
                if(h < this->minimumSize().height()){
                    h = this->minimumSize().height();
                }
                break;
        case left:
                x += offset_left_x;
                w -= offset_left_x;
                if(w < this->minimumSize().width()){
                    x += w - this->minimumSize().width();
                    w = this->minimumSize().width();
                }
                break;
        default:
            break;
        }
        this->setGeometry(x, y, w, h);
    }
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
    if(isDraging){
        setCursor(Qt::ArrowCursor);
        cursorState = normal;
        isDraging = false;
        _oldPos = this->pos();
    }
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

void MainWidget::dealCloWindow(){
    this->close();
}

void MainWidget::dealMinWindow(){
    this->showMinimized();
}

void MainWidget::dealMovWindow(QPoint p){
    this->move(p);
}

void MainWidget::detectEdge(const QPoint& cpos){
    int state = normal;
    if(abs(cpos.x() - 0) < winRszTld)
        state |= left;
    else if(abs(cpos.x() - this->geometry().width()) < winRszTld)
        state |= right;
    if(abs(cpos.y() - 0) < winRszTld)
        state |= top;
    else if(abs(cpos.y() - this->geometry().height()) < winRszTld)
        state |= bottom;
    cursorState = CursorState(state);
    switch(cursorState){
    case leftTop:
    case rightBottom:
            setCursor(Qt::SizeFDiagCursor);
            break;
    case top:
    case bottom:
            setCursor(Qt::SizeVerCursor);
            break;
    case rightTop:
    case leftBottom:
            setCursor(Qt::SizeBDiagCursor);
            break;
    case right:
    case left:
            setCursor(Qt::SizeHorCursor);
            break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}
