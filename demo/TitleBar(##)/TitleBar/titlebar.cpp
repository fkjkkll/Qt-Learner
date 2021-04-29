#include "titlebar.h"
#include "ui_titlebar.h"
#include <QMouseEvent>
#include <QResizeEvent>
#include <QScreen>
#include <QEvent>

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
}

TitleBar::~TitleBar()
{
    delete ui;
    delete icon;
    delete offset;
}

void TitleBar::on_tbtMin_clicked(){ // 最小化窗口
    emit this->minWindow();
}

void TitleBar::on_tbtMax_clicked(){ // 最大化窗口和向下还原窗口
    isWinMax = !isWinMax;
    if(isWinMax){   // 最大化窗口
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
}

void TitleBar::on_tbtClo_clicked(){ // 关闭窗口
    emit this->cloWindow();
}

void TitleBar::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        if(isWinMax)   // 从[最大化]拖到[向下还原]
            on_tbtMax_clicked();
        emit this->movWindow(event->globalPos() - *offset); // 常规移动
    }
}

void TitleBar::mousePressEvent(QMouseEvent *event){
    if(isWinMax){   // 从[最大化]拖到[向下还原]的移动
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect mm = screen->availableGeometry() ;
        int screen_width = mm.width();
        int xpos = (event->globalPos().x())<(_oldSize.width()/2)?
                    (event->globalPos().x()):(_oldSize.width()/2);
        xpos = (screen_width - event->globalPos().x())<(_oldSize.width()/2)?
                    (_oldSize.width() + event->globalPos().x() - screen_width):xpos;
        // xpos调整[向下还原]的初始位置
        *offset = QPoint(xpos, event->pos().y());
    }
    else    // 正常情况下的移动
        *offset = event->pos();
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event){
    if(event->globalPos().y() == 0)   // 鼠标拖到顶端进入[最大化]
        on_tbtMax_clicked();
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event){
    Q_UNUSED(event);
    on_tbtMax_clicked();    // 最大化窗口和向下还原窗口
}

void TitleBar::resizeEvent(QResizeEvent *event){
    _oldSize = event->oldSize();    // 记录标题栏大小改变前的大小
}

bool TitleBar::eventFilter(QObject *watched, QEvent *event){
    if((watched == ui->tbtClo ||
        watched == ui->tbtMax ||
        watched == ui->tbtMin)&& event->type() == QEvent::MouseMove)  // 防止拖拽按钮也可以拖拽窗体
        return true;

    return QWidget::eventFilter(watched, event);
}
