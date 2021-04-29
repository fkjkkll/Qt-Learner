#include "fullscreenwidget.h"
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include "windows.h"
#include <QDebug>
#define cout qDebug() << "[" << __FILE__ <<":" << __LINE__ << "]"
FullScreenWidget::FullScreenWidget(QWidget *parent): QWidget(parent){
    setWindowState(Qt::WindowActive|Qt::WindowFullScreen);
    tipWidth = 300; //��ܰ��ʾ��Ŀ��
    tipHeight = 100; //��ܰ��ʾ��ĸ߶�
    infoWidth = 100; //������Ϣ��Ŀ��
    infoHeight = 50; //������Ϣ��ĸ߶�
    initSelectedMenu();
    initFullScreenWidget();
}

void FullScreenWidget::initFullScreenWidget(){
    currentShotState = initShot;
    controlValue = moveControl0;
    beginPoint =QPoint(0,0);
    endPoint = QPoint(0,0);
    moveBeginPoint = QPoint(0,0);
    moveEndPoint = QPoint(0,0);

    tlRect = QRect(0,0,0,0); //���ϵ�
    trRect = QRect(0,0,0,0); //���ҵ�
    blRect = QRect(0,0,0,0); //���µ�
    brRect = QRect(0,0,0,0); //���µ�
    tcRect = QRect(0,0,0,0); //���е�
    bcRect = QRect(0,0,0,0); //���е�
    lcRect = QRect(0,0,0,0); //���е�
    rcRect = QRect(0,0,0,0); //���е�

    setCursor(Qt::CrossCursor);//�ı������ò
}

void FullScreenWidget::initSelectedMenu(){
    savePixmapAction = new QAction(QStringLiteral("����ѡ��"),this);
    cancelAction = new QAction(QStringLiteral("��ѡ"),this);
    quitAction = new QAction(QStringLiteral("�˳�"),this);
    contextMenu = new QMenu(this);

    connect(savePixmapAction,SIGNAL(triggered()),this,SLOT(savePixmap()));
    connect(cancelAction,SIGNAL(triggered()),this,SLOT(cancelSelectedRect()));
    connect(quitAction, QAction::triggered,[=](){
                hide();
                emit mainwinshow();
            });
}

void FullScreenWidget::savePixmap(){
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, QStringLiteral("����ͼƬ"),
               QDir::currentPath(), tr("Images (*.jpg *.png *.bmp)"));
    if(fileName.isNull())
    return;

    shotPixmap.save(fileName);
    hide();
    emit mainwinclose();
}

QPixmap FullScreenWidget::getFullScreenPixmap(){
    emit mainwinhide();
    Sleep(50);//�ȴ�50ms
    initFullScreenWidget();
    //QScreen* screen = QGuiApplication::primaryScreen();
    QScreen* screen = QApplication::screens().last();
    QPixmap result;
    result = screen->grabWindow(QApplication::desktop()->winId()); //ץȡ��ǰ��Ļ��ͼƬ
    return result;
}

void FullScreenWidget::loadBackgroundPixmap(const QPixmap &bgPixmap){   //�ۺ���
    QDesktopWidget *pDecktopWidget = QApplication::desktop();
    //���漸�����ǳ�ʼ��
    screenx = 0;
    screeny = 0;
    screenwidth = pDecktopWidget->screen()->width();
    screenheight = pDecktopWidget->screen()->height();
    loadPixmap = bgPixmap;
    initFullScreenWidget();
}

void FullScreenWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QColor shadowColor;
    shadowColor= QColor(0,0,0,100); //��Ӱ��ɫ����
    painter.begin(this); //�����ػ�
    painter.setPen(QPen(Qt::blue,2,Qt::SolidLine,Qt::FlatCap));//���û���
    painter.drawPixmap(screenx,screeny,loadPixmap); //������ͼƬ����������
    painter.fillRect(screenx,screeny,screenwidth,screenheight,shadowColor); //��Ӱ��Ч��
    switch(currentShotState){
        case initShot:
            drawTipsText();
            break;

        case beginShot:
        case finishShot: //
            selectedRect = getSelectedRect(); //��ȡѡ�� getSelectedRect
            drawSelectedPixmap();
            break;

        case beginMoveShot:
        case finishMoveShot: //getMoveAllSelectedRect
            selectedRect = getSelectedRect(); //��ȡѡ�� getSelectedRect
            drawSelectedPixmap();
            break;

        case beginControl:
        case finishControl: //getMoveControlSelectedRect
            selectedRect = getMoveControlSelectedRect(); //��ȡѡ�� getSelectedRect
            drawSelectedPixmap();
            break;

        default:
            break;
    }
    drawXYWHInfo(); //��ӡ������Ϣ
    painter.end();  //�ػ����
    if(currentShotState == finishMoveShot || currentShotState == finishControl){       
        updateBeginEndPointValue(selectedRect); //���ƶ���ѡ���󣬸���beginPoint,endPoint;Ϊ��һ���ƶ���׼������
        currentShotState = finishShot;
    }
}

void FullScreenWidget::updateBeginEndPointValue(const QRect &rect){
    beginPoint = rect.topLeft();
    /*
     * bottomRight: Note that for historical reasons this function
     * returns QPoint(left() + width() -1, top() + height() - 1).
    */
    endPoint.setX(rect.bottomRight().x() + 1);
    endPoint.setY(rect.bottomRight().y() + 1);
   // endPoint = rect.bottomRight(); //����
    moveBeginPoint = QPoint(0,0);
    moveEndPoint = QPoint(0,0);
}

void FullScreenWidget::mousePressEvent(QMouseEvent *event){
    //����ʼ�����϶�����ѡ������ʱ,ȷ����ʼѡȡ��beginPoint����
    if(event->button() == Qt::LeftButton && currentShotState == initShot){
        currentShotState = beginShot; //���õ�ǰ״̬ΪbeginShot״̬
        beginPoint = event->pos();
        endPoint = event->pos();//��ʼ������Ȼ���ڿ��ܻ�����(printevent̫����)
    }
    //�ƶ�ѡ���ı�ѡ��������λ��
    if(event->button() == Qt::LeftButton && isInSelectedRect(event->pos()) &&
     getMoveControlState(event->pos()) == moveControl0){
        currentShotState = beginMoveShot; //���ÿ�ʼ�ƶ�ѡȡѡ��,beginMoveShot״̬
        moveBeginPoint = event->pos();
        moveEndPoint = event->pos();//��ʼ������Ȼ���ڿ��ܻ�����(printevent̫����)
    }
    //�ƶ����Ƶ�ı�ѡ����С
    if(event->button() == Qt::LeftButton && getMoveControlState(event->pos()) != moveControl0){
        currentShotState = beginControl; //��ʼ�ƶ����Ƶ�
        controlValue = getMoveControlState(event->pos());
        moveBeginPoint = event->pos();
        moveEndPoint = event->pos();//��ʼ������Ȼ���ڿ��ܻ�����(printevent̫����)
    }
}

void FullScreenWidget::mouseReleaseEvent(QMouseEvent *event){
    // ��ǰ�ǵ�һ�ν�ȡѡȡ���ɿ����ʱ
    if(event->button() == Qt::LeftButton && currentShotState == beginShot){
        currentShotState = finishShot;
        endPoint = event->pos();
        update();
    }
    // ��ǰ�ƶ�ѡ�����ɿ����ʱ
    if(event->button() == Qt::LeftButton && currentShotState == beginMoveShot){
        currentShotState = finishMoveShot;
        moveEndPoint = event->pos();
        update();
    }

    // ��ǰͨ�����Ƶ�ı�ѡ������ǰ״̬ΪbeginControl״̬ʱ������״̬ΪfinishControl
    if(event->button() == Qt::LeftButton && currentShotState == beginControl){
        currentShotState = finishControl;
        moveEndPoint = event->pos();
        update();
    }
}

void FullScreenWidget::mouseMoveEvent(QMouseEvent *event){
    //���϶�ʱ����̬�ĸ�����ѡ�������
    if(currentShotState == beginShot){
        endPoint = event->pos();
        update();
    }

    //��ȷ��ѡ���󣬶�ѡ�������ƶ�����
    if(currentShotState == beginMoveShot || currentShotState == beginControl){
        moveEndPoint = event->pos();
        update();
    }
    updateMouseShape(event->pos()); //�޸�������״
    setMouseTracking(true);
}

QRect FullScreenWidget::getRect(const QPoint &bPoint, const QPoint &ePoint){
    int x,y,width,height;
    width = qAbs(bPoint.x() - ePoint.x());
    height = qAbs(bPoint.y() - ePoint.y());
    x = bPoint.x() < ePoint.x() ? bPoint.x() : ePoint.x();
    y = bPoint.y() < ePoint.y() ? bPoint.y() : ePoint.y();
    return QRect(x,y,width,height);
}

bool FullScreenWidget::isInSelectedRect(const QPoint &point){
    int x,y;
    QRect seleRect;
    if(currentShotState == initShot || currentShotState == beginShot)
        return false;

    seleRect = getSelectedRect();
    x = point.x();
    y = point.y();

    return seleRect.contains(x,y);
}

QRect FullScreenWidget::getSelectedRect(){
    if(currentShotState == beginMoveShot || currentShotState == finishMoveShot){
        return getMoveAllSelectedRect();
    }
    else if(currentShotState == beginControl || currentShotState == finishControl){
        return getMoveControlSelectedRect();
    }
    else{
        return getRect(beginPoint, endPoint);
    }
}

void FullScreenWidget::checkMoveEndPoint(){
    //�����涼�Ƕ�ֵ������moveEndPoint
    int x,y;
    QRect seleRect = getRect(beginPoint, endPoint);
    QPoint bottomRightPoint = seleRect.bottomRight();
    x = moveEndPoint.x() - moveBeginPoint.x();//��ֵ
    y = moveEndPoint.y() - moveBeginPoint.y();
    if(x + seleRect.x() < 0){ //���ƶ���X����С����ʱ�������ѡ����ʧ��������moveEndPoint��X�������ֵ�����и�ֵ
        moveEndPoint.setX(qAbs(seleRect.x() - moveBeginPoint.x()));
    }
    if(y + seleRect.y() < 0){ //���ƶ���Y����С����ʱ�������ѡ����ʧ��������moveEndPoint��Y�������ֵ�����и�ֵ
        moveEndPoint.setY(qAbs(seleRect.y() - moveBeginPoint.y()));
    }
    if(x + bottomRightPoint.x() > screenwidth){ //���ƶ�ѡ���󣬳��ֳ���������Ļ������ʱ������moveEndPoint��X���������
        moveEndPoint.setX(screenwidth - (bottomRightPoint.x() - moveBeginPoint.x()));
    }
    if(y + bottomRightPoint.y() > screenheight){ //���ƶ�ѡ���󣬳��ֳ���������Ļ������ʱ������moveEndPoint��Y���������ֵ
        moveEndPoint.setY(screenheight - (bottomRightPoint.y() - moveBeginPoint.y()));
    }
}

QRect FullScreenWidget::getMoveAllSelectedRect(void){
    QRect result;
    QPoint tmpBeginPoint,tmpEndPoint;
    int moveX,moveY;
    checkMoveEndPoint(); //���ƶ�ѡ�������жϣ����ƶ���ѡ�������߽磬��ֹͣ�ƶ�
    moveX = moveEndPoint.x() - moveBeginPoint.x();
    moveY = moveEndPoint.y() - moveBeginPoint.y();
    tmpBeginPoint.setX(beginPoint.x() + moveX);
    tmpBeginPoint.setY(beginPoint.y() + moveY);
    tmpEndPoint.setX(endPoint.x() + moveX);
    tmpEndPoint.setY(endPoint.y() + moveY);
    result = getRect(tmpBeginPoint, tmpEndPoint);
    return result;
}

void FullScreenWidget::checkControlPoint(){

    if(moveEndPoint.x()<0)
        moveEndPoint.setX(0);
    if(moveEndPoint.y()<0)
        moveEndPoint.setY(0);
    if(moveEndPoint.x()>screenwidth)
        moveEndPoint.setX(screenwidth);
    if(moveEndPoint.y()>screenheight)
        moveEndPoint.setY(screenheight);
}

QRect FullScreenWidget::getMoveControlSelectedRect(void){
    int x,y,w,h;
    QRect rect = getRect(beginPoint, endPoint);//ԭ���Ѿ�ѡ�õ�ѡ��
    QRect result;
    checkControlPoint(); //����ק��ѡ�������жϣ����ƶ���ѡ�������߽磬��ֹͣ�ƶ�
    switch(controlValue){
    case moveControl1:
        result = getRect(rect.bottomRight(), moveEndPoint);
        return result;
        break;
    case moveControl2:
        x = rect.x();
        y = getMinValue(moveEndPoint.y(),rect.bottomLeft().y());
        w = rect.width();
        h = qAbs(moveEndPoint.y() - rect.bottomRight().y());
        break;
    case moveControl3:
        result = getRect(rect.bottomLeft(),moveEndPoint);
        return result;
        break;
    case moveControl4:
        x = getMinValue(rect.x(),moveEndPoint.x());
        y = rect.y();
        w = qAbs(rect.bottomLeft().x() - moveEndPoint.x());
        h = rect.height();
        break;
    case moveControl5:
        result = getRect(rect.topLeft(),moveEndPoint);
        return result;
        break;
    case moveControl6:
        x = rect.x();
        y = getMinValue(rect.y(),moveEndPoint.y());
        w = rect.width();
        h = qAbs(moveEndPoint.y() - rect.topLeft().y());
        break;
    case moveControl7:
        result = getRect(moveEndPoint,rect.topRight());
        return result;
        break;
    case moveControl8:
        x = getMinValue(moveEndPoint.x(),rect.bottomRight().x());
        y = rect.y();
        w = qAbs(rect.bottomRight().x() - moveEndPoint.x());
        h = rect.height();
        break;
    default:
        result = getRect(beginPoint,endPoint);
        return result;
        break;
    }
    return QRect(x,y,w,h); //��ȡѡ��
}

int FullScreenWidget::getMinValue(int num1, int num2){
    return num1<num2?num1:num2;
}

void FullScreenWidget::drawSelectedPixmap(void){
    painter.drawRect(selectedRect); //��ѡ�еľ��ο�
    shotPixmap = loadPixmap.copy(selectedRect);  //����ѡ����Pixmap
    if(selectedRect.width() > 0 && selectedRect.height()){
        painter.drawPixmap(selectedRect.topLeft(),shotPixmap); //��ѡ�������ͼƬ
    }
    draw8ControlPoint(selectedRect); //����ѡ����8�����Ƶ�
}

void FullScreenWidget::drawXYWHInfo(void){
    int x,y;
    QColor color = QColor(239,234,228,200);
    QRect rect;
    QString strTipsText;
    switch(currentShotState){
        case beginShot:
        case finishShot:
        case beginMoveShot:
        case finishMoveShot:
        case beginControl:
        case finishControl:
            x = selectedRect.x() + 20;
            y = selectedRect.y() > infoHeight ? selectedRect.y()-infoHeight:selectedRect.y();
            rect = QRect(x,y,infoWidth,infoHeight);
            strTipsText = QString(QStringLiteral(" ������Ϣ\n x:%1 y:%2\n w:%3 h:%4"))
                    .arg(selectedRect.x(),4).arg(selectedRect.y(),4)
                    .arg(selectedRect.width(),4).arg(selectedRect.height(),4);
            painter.fillRect(rect, color);
            painter.setPen(QPen(Qt::black));//���û��ʵ���ɫΪ��ɫ
            painter.drawText(rect,Qt::AlignLeft|Qt::AlignVCenter,strTipsText);
            break;
        default:
            break;
    }
}

FullScreenWidget::controlPointEnum FullScreenWidget::getMoveControlState(const QPoint &point){
    FullScreenWidget::controlPointEnum result = moveControl0;
    if(currentShotState == initShot || currentShotState == beginShot)  {
        result = moveControl0;
    }
    else if(tlRect.contains(point)){
        result = moveControl1;
    }
    else if(tcRect.contains(point)){
        result = moveControl2;
    }
    else if(trRect.contains(point)){
        result = moveControl3;
    }
    else if(rcRect.contains(point)){
        result = moveControl4;
    }
    else if(brRect.contains(point)){
        result = moveControl5;
    }
    else if(bcRect.contains(point)){
        result = moveControl6;
    }
    else if(blRect.contains(point)){
        result = moveControl7;
    }
    else if(lcRect.contains(point)){
        result = moveControl8;
    }
    else{
        result = moveControl0;
    }
    return result;
}

void FullScreenWidget::draw8ControlPoint(const QRect &rect){
    int x,y;
    QColor color= QColor(0,0,255); //�������ɫ����
    QPoint tlPoint = rect.topLeft(); //���ϵ�
    QPoint trPoint = rect.topRight(); //���ϵ�
    QPoint blPoint = rect.bottomLeft(); //���µ�
    QPoint brPoint = rect.bottomRight(); //���µ�

    x = (tlPoint.x()+trPoint.x())/2;
    y = tlPoint.y();
    QPoint tcPoint = QPoint(x,y);

    x = (blPoint.x()+brPoint.x())/2;
    y = blPoint.y();
    QPoint bcPoint = QPoint(x,y);

    x = tlPoint.x();
    y = (tlPoint.y()+blPoint.y())/2;
    QPoint lcPoint = QPoint(x,y);

    x = trPoint.x();
    y = (trPoint.y()+brPoint.y())/2;
    QPoint rcPoint = QPoint(x,y);

    tlRect = QRect(tlPoint.x()-2,tlPoint.y()-2,6,6); //���ϵ�
    trRect = QRect(trPoint.x()-2,trPoint.y()-2,6,6); //���ϵ�
    blRect = QRect(blPoint.x()-2,blPoint.y()-2,6,6); //���µ�
    brRect = QRect(brPoint.x()-2,brPoint.y()-2,6,6); //���µ�
    tcRect = QRect(tcPoint.x()-2,tcPoint.y()-2,6,6); //���е�
    bcRect = QRect(bcPoint.x()-2,bcPoint.y()-2,6,6); //���е�
    lcRect = QRect(lcPoint.x()-2,lcPoint.y()-2,6,6);//���е�
    rcRect = QRect(rcPoint.x()-2,rcPoint.y()-2,6,6); //���е�

    painter.fillRect(tlRect,color);
    painter.fillRect(trRect,color);
    painter.fillRect(blRect,color);
    painter.fillRect(brRect,color);
    painter.fillRect(tcRect,color);
    painter.fillRect(bcRect,color);
    painter.fillRect(lcRect,color);
    painter.fillRect(rcRect,color);
}

void FullScreenWidget::updateMouseShape(const QPoint &point){
    switch(currentShotState){
        case initShot:
        case beginShot:
            setCursor(Qt::CrossCursor);
            break;
        case beginMoveShot:
            setCursor(Qt::OpenHandCursor);
            break;
        case finishShot:
        case finishMoveShot:
        case finishControl:
            if(isInSelectedRect(point))
                setCursor(Qt::OpenHandCursor);
            else
                updateMoveControlMouseShape(getMoveControlState(point));
            break;
        case beginControl:
            updateMoveControlMouseShape(controlValue); //���ú������ƶ�8�����Ƶ�������״̬�ĸı�
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
    }
}

void FullScreenWidget::updateMoveControlMouseShape(controlPointEnum controlValue){
  switch(controlValue){
    case moveControl1:
    case moveControl5:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case moveControl2:
    case moveControl6:
        setCursor(Qt::SizeVerCursor);
        break;
    case moveControl3:
    case moveControl7:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case moveControl4:
    case moveControl8:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
  }
}

void FullScreenWidget::drawTipsText(){
    int x = (screenwidth - tipWidth)/2;
    int y = (screenheight - tipHeight)/2;
    QColor color = QColor(100,100,100,200);
    QRect rect = QRect(x,y,tipWidth,tipHeight);
//    QString strTipsText = QString(QStringLiteral("��ܰ��ʾ\n����϶����н���;�����������Ҽ�����;\n�����������Ҽ�ȡ��;ESC�˳�;"));
    QString strTipsText = QString("��ܰ��ʾ\n����϶����н���;�����������Ҽ�����;\n�����������Ҽ�ȡ��;ESC�˳�;");
    painter.fillRect(rect,color);
    painter.setPen(QPen(Qt::white));//���û��ʵ���ɫΪ��ɫ
    painter.drawText(rect,Qt::AlignCenter,strTipsText);

}

void FullScreenWidget::contextMenuEvent(QContextMenuEvent *event){
    contextMenu->clear();
    if(isInSelectedRect(event->pos())){
        contextMenu->addAction(savePixmapAction);
    }
    else{
        contextMenu->addAction(cancelAction);
        contextMenu->addAction(quitAction);
    }
    contextMenu->exec(event->pos());
}

void FullScreenWidget::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
        initFullScreenWidget();
        hide();
        emit mainwinshow();
    }
}

void FullScreenWidget::mouseDoubleClickEvent(QMouseEvent *event){
    Q_UNUSED(event);
    if(currentShotState == finishShot || currentShotState == finishMoveShot || currentShotState == finishControl){
        hide();
        emit mainwinshow();
    }
}

void FullScreenWidget::cancelSelectedRect(){
    initFullScreenWidget();
    update(); //�����ػ棬��ѡȡ����ȥ��
}
