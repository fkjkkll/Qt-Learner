#ifndef WATERWAVEBTN_H
#define WATERWAVEBTN_H

#include <QWidget>
#include <QLabel>
#include <QMovie>

class QMovie;

class WaterWaveBtn : public QWidget{
	Q_OBJECT

public:
    WaterWaveBtn(const QString& pngFile, QWidget *parent = 0);
	~WaterWaveBtn();

private slots:
	void showWaterWave();

private:
    QLabel*	waterWaveLabel;
    QMovie* m_Movie;
	
};

#endif // WATERWAVEBTN_H
