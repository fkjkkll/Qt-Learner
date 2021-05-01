#include "titlebar.h"
#include "ui_titlebar.h"
#include <QMouseEvent>
#include <QResizeEvent>
#include <QScreen>
#include <QEvent>
#include <QDebug>
#define cout qDebug() << "[" << __FILE__ <<":" << __LINE__ << "]"

TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleBar)
{
    ui->setupUi(this);
    // 初始设定
    this->setFixedHeight(iconHeight);
    this->setWindowFlags(Qt::FramelessWindowHint);
    // 标题栏
    ui->labTit->setFont(QFont("微软雅黑"));
    ui->labTit->setText("Customized Title Bar");
    // 设置图标大小
    ui->labPic->setFixedSize(iconWidth, iconHeight);
    ui->tbtMin->setFixedSize(iconWidth, iconHeight);
    ui->tbtMax->setFixedSize(iconWidth, iconHeight);
    ui->tbtClo->setFixedSize(iconWidth, iconHeight);
    // 设置图标
    icon = new QPixmap("://mysql.png");
    *icon = icon->scaled(iconHeight, iconHeight);
    ui->labPic->setPixmap(*icon);
    ui->labPic->setAlignment(Qt::AlignCenter);
    icon->load("://min1.png");
    ui->tbtMin->setIcon(QIcon(*icon));
    ui->tbtMin->setToolTip("最小化");
    icon->load("://max2.png");
    ui->tbtMax->setIcon(QIcon(*icon));
    ui->tbtMax->setToolTip("最大化");
    icon->load("://close2.png");
    ui->tbtClo->setIcon(QIcon(*icon));
    ui->tbtClo->setToolTip("关闭");

    // 样式   最顶层窗口的样式设置不能用QSS，总是会出现莫名其妙的问题:(
    this->setPalette(QPalette(QColor(153,165,189)));
    this->setAutoFillBackground(true);
    this->setStyleSheet("QToolButton{background:transparent; border:none;}"
                        "QToolButton:hover{background-color:rgb(126,126,126);}"
                        "QToolButton:hover#tbtClo{background-color:rgb(255,0,0);}");
    // 移动
    offset = new QPoint(0,0);

    // 防止拖拽按钮也可以拖拽窗体
    ui->tbtClo->installEventFilter(this);
    ui->tbtMax->installEventFilter(this);
    ui->tbtMin->installEventFilter(this);
    this->installEventFilter(this);

    // 标题栏及其子控件设定鼠标自动追踪
    this->setMouseTracking(true);
    ui->labPic->setMouseTracking(true);
    ui->labTit->setMouseTracking(true);
    ui->tbtClo->setMouseTracking(true);
    ui->tbtMax->setMouseTracking(true);
    ui->tbtMin->setMouseTracking(true);
}

TitleBar::~TitleBar(){
    delete ui;
    delete icon;
    delete offset;
}

void TitleBar::on_tbtMin_clicked(){ // 最小化窗口
    emit this->minWindow();
}

void TitleBar::on_tbtClo_clicked(){ // 关闭窗口
    emit this->cloWindow();
}

void TitleBar::on_tbtMax_clicked(){ // 最大化窗口和向下还原窗口
    if(!isWinMax){   // 最大化窗口
        emit this->maxWindow();
        icon->load("://huanyuan.png");
        ui->tbtMax->setIcon(QIcon(*icon));
        ui->tbtMax->setToolTip("向下还原");
    }
    else{   // 向下还原窗口
        emit this->unmaxWindow();
        icon->load("://max2.png");
        ui->tbtMax->setIcon(QIcon(*icon));
        ui->tbtMax->setToolTip("最大化");
    }
    isWinMax = !isWinMax;
}

void TitleBar::mouseMoveEvent(QMouseEvent *event){  // 鼠标移动事件
    if(event->buttons() & Qt::LeftButton && canMov){
        isMoving = true;
        if(isWinMax)   // 从[最大化]拖到[向下还原]
            on_tbtMax_clicked();
        emit this->movWindow(event->globalPos() - *offset); // 常规移动
    }
    if(!isMoving){  // 用于父窗口[拖拽改变大小]
        QMouseEvent *mEvent = new QMouseEvent(QEvent::MouseMove, event->pos(),
                                              event->button(), event->buttons(), Qt::NoModifier);
        QApplication::postEvent(this->parentWidget(), mEvent);  // 事件穿透：发给父窗口
    }
}

void TitleBar::mousePressEvent(QMouseEvent *event){ // 鼠标按下事件
    QMouseEvent *mEvent = new QMouseEvent(QEvent::MouseButtonPress, event->pos(),
                                          event->button(), event->buttons(), Qt::NoModifier);
    QApplication::postEvent(this->parentWidget(), mEvent);  // 事件穿透：发给父窗口
    if(isWinMax){   // 从[最大化]拖到[向下还原]的移动
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect mm = screen->availableGeometry() ;
        int screen_width = mm.width();
        int xpos = (event->globalPos().x())<(_oldSize.width()/2)?
                    (event->globalPos().x()):(_oldSize.width()/2);
        xpos = (screen_width - event->globalPos().x())<(_oldSize.width()/2)?
                    (_oldSize.width() + event->globalPos().x() - screen_width):xpos;
        // xpos调整[向下还原]的初始位置: 偏移坐标
        *offset = QPoint(xpos, event->pos().y());

    }
    else    // 正常情况下的移动: 偏移坐标
        *offset = event->pos();
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event){   // 鼠标释放事件
    QMouseEvent *mEvent = new QMouseEvent(QEvent::MouseButtonRelease, event->pos(),
                                          event->button(), event->buttons(), Qt::NoModifier);
    QApplication::postEvent(this->parentWidget(), mEvent);  // 事件穿透：发给父窗口
    if(event->globalPos().y() == 0 && !isWinMax && event->pos().y()>=0 &&
            this->parentWidget()->cursor().shape()==Qt::ArrowCursor)   // 鼠标拖到顶端进入[最大化]
        on_tbtMax_clicked();
    isMoving = false;
}

void TitleBar::resizeEvent(QResizeEvent *event){
    _oldSize = event->oldSize();    // 记录标题栏大小改变前的大小
}

bool TitleBar::eventFilter(QObject *watched, QEvent *event){    // 事件过滤器
    // 防止拖拽按钮也可以拖拽窗体
    // 1、解决拖动按钮也能拖动窗口的问题
    // 2、解决[拖动改变大小]拖动按钮也能改变大小的问题
    if((watched == ui->tbtClo ||
        watched == ui->tbtMax ||
        watched == ui->tbtMin) && event->type() == QEvent::MouseMove){
        if(!pressingBtn){   // 如果标题栏的按钮未被按下，发送带有按钮信息的事件：用于[拖拽改变大小]和[改变鼠标形状状态]
            QMouseEvent *mEvent = new QMouseEvent(QEvent::MouseMove, TitleBar::mapFromGlobal(QCursor::pos()),
                                                  static_cast<QMouseEvent*>(event)->button(),
                                                  static_cast<QMouseEvent*>(event)->buttons(), Qt::NoModifier);
            QApplication::postEvent(this->parentWidget(), mEvent);  // 事件穿透：直接发给父窗口的mouseMoveEvent，连带按钮消息
        }
        return true;    // 其他控件不需要捕捉位于按钮上的移动事件了
    }

    // 防止[双击放大]夹带[单击拖拽移动]
    if(watched == this && event->type() == QEvent::MouseButtonDblClick){
        on_tbtMax_clicked();    // 双击[最大化]
        canMov = false;
        return true;
    }

    // 调整[拖拽最大化][向下还原]的位置
    if(event->type() == QEvent::MouseButtonPress && !isWinMax){ // 非最大化状态下
        emit this->recordOldPos();  // 发送信号让主窗口记录当前窗体位置，留作备份
        if(this->parentWidget()->cursor().shape() != Qt::ArrowCursor){  // 主窗体可能要[拖拽改变大小]，此时不能移动
            canMov = false;
        }
        if(watched == ui->tbtClo || watched == ui->tbtMax || watched == ui->tbtMin){
            if(this->parentWidget()->cursor().shape() == Qt::ArrowCursor)
                pressingBtn = true;
            QMouseEvent *mEvent = new QMouseEvent(QEvent::MouseButtonPress, TitleBar::mapFromGlobal(QCursor::pos()),
                                                  static_cast<QMouseEvent*>(event)->button(),
                                                  static_cast<QMouseEvent*>(event)->buttons(), Qt::NoModifier);
            QApplication::postEvent(this, mEvent);  // 事件穿透：发给本层窗口
            if(this->parentWidget()->cursor().shape() != Qt::ArrowCursor){  // 主窗体可能要[拖拽改变大小]，此时不能触发按钮效果
                return true;
            }
        }
    }
    else if(event->type() == QEvent::MouseButtonRelease){
        canMov = true;
        pressingBtn = false;
        if(watched == ui->tbtClo || watched == ui->tbtMax || watched == ui->tbtMin){
            QMouseEvent *mEvent = new QMouseEvent(QEvent::MouseButtonRelease, TitleBar::mapFromGlobal(QCursor::pos()),
                                                  static_cast<QMouseEvent*>(event)->button(),
                                                  static_cast<QMouseEvent*>(event)->buttons(), Qt::NoModifier);
            QApplication::postEvent(this, mEvent);  // 事件穿透：发给本层窗口
        }
    }
    return QWidget::eventFilter(watched, event);
}
