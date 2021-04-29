#ifndef SCREENSHOTWINDOW_H
#define SCREENSHOTWINDOW_H

#include "fullscreenwidget.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QEvent>
#include <QKeyEvent>
namespace Ui {
class ScreenShotWindow;
}

class ScreenShotWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit ScreenShotWindow(QWidget *parent = 0);
    ~ScreenShotWindow();

public slots:
    //进行截屏的槽函数
    void screenshot();
    void forshow();
    void forhide();

signals:
    // 设置pixmap信息
    void setPixmap(QPixmap pixmap);

private:
    Ui::ScreenShotWindow *ui;

    QToolBar *mainToolBar;
    QAction *newAct;
    QVBoxLayout *mainLayout;
    QPixmap fullPixmap; //全屏图片
    FullScreenWidget *fullWidget;

    QPoint* pos_offset;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
protected:
    bool eventFilter(QObject *watched, QEvent *event);


};

#endif // SCREENSHOTWINDOW_H
