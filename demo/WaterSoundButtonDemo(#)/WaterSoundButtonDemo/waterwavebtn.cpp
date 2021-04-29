#include "waterwavebtn.h"
#include "labelbtn.h"
#include <QDebug>
#define cout qDebug() << "[" << __FILE__ <<":" << __LINE__ << "]"

WaterWaveBtn::WaterWaveBtn(const QString& pngFile, QWidget *parent)
    : QWidget(parent)
{
    int size = QPixmap(":/Resource/waterWave.gif").width();         //240
    m_Movie = new QMovie(":/Resource/waterWave.gif");
    m_Movie->setCacheMode(QMovie::CacheAll);
    waterWaveLabel = new QLabel(this);
    waterWaveLabel->setMovie(m_Movie);
    waterWaveLabel->setAttribute(Qt::WA_TranslucentBackground,true);
    waterWaveLabel->setFixedSize(size, size);

//    m_Movie->start(); //不用预先设置movie底下lable的大小了
//    m_Movie->setPaused(true);

    LabelBtn* labelBtn = new LabelBtn(pngFile,":/Resource/sound.wav",this);
    int gap = (size - labelBtn->width())/2;                         //20
    labelBtn->move(gap,gap);
	this->setFixedSize(size,size);
    this->setAttribute(Qt::WA_TranslucentBackground);

    connect(labelBtn, SIGNAL(clicked()), this, SLOT(showWaterWave()));
}

WaterWaveBtn::~WaterWaveBtn(){

}

void WaterWaveBtn::showWaterWave(){
	m_Movie->start();
}
