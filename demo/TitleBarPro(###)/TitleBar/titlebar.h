#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QFont>

namespace Ui {
class TitleBar;
}

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = 0);
    bool isMaxWindow(){return isWinMax;}
    ~TitleBar();

signals:
    void minWindow();           // 让主窗体最小化
    void maxWindow();           // 让主窗体最大化
    void unmaxWindow();         // 让主窗体向下还原
    void cloWindow();           // 让主窗体关闭
    void movWindow(QPoint);     // 让主窗体移动
    void recordOldPos();        // 让主窗体记录当前窗体坐标

private slots:
    void on_tbtMin_clicked();
    void on_tbtMax_clicked();
    void on_tbtClo_clicked();

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::TitleBar *ui;
    QPixmap *icon;
    int iconWidth = 50;
    int iconHeight = 30;
    QPoint *offset;                  // 窗体移动，相对偏移量
    QSize _oldSize;                  // 窗体前一刻大小
    bool isWinMax = false;           // 窗体是否最大化
    bool canMov = true;              // 窗体是否可移动
    bool isMoving = false;           // 窗体是否正在移动
    bool pressingBtn = false;        // 标题栏的按钮是否正被按下
};

#endif // WIDGET_H
