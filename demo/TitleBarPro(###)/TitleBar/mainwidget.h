#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    enum CursorState{   // 表示通过鼠标[拖拽改变大小]的状态
        normal = 0b0000,
        leftTop = 0b1001 ,
        top = 0b0001,
        rightTop = 0b0011,
        right = 0b0010,
        rightBottom = 0b0110,
        bottom = 0b0100,
        leftBottom = 0b1100,
        left = 0b1000
    };

public slots:
    void dealCloWindow();
    void dealMaxWindow();
    void dealUnmaxWindow();
    void dealMinWindow();
    void dealMovWindow(QPoint);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    void detectEdge(const QPoint&);         // [拖拽改变大小]检测边

private:
    Ui::MainWidget *ui;
    QSize _oldSize;
    QPoint _oldPos;
    CursorState cursorState;    // 鼠标状态
    const int winRszTld = 10;   // [拖拽改变大小]与边的距离阈值
    bool isDraging = false;     // 是否正在[拖拽改变大小]
};

#endif // MAINWIDGET_H
