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
    ~TitleBar();

signals:
    void minWindow();
    void maxWindow();
    void unmaxWindow();
    void cloWindow();
    void movWindow(QPoint);

private slots:
    void on_tbtMin_clicked();
    void on_tbtMax_clicked();
    void on_tbtClo_clicked();

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::TitleBar *ui;
    QPixmap *icon;
    int iconWidth = 50;
    int iconHeight = 30;
    bool isWinMax = false;
    QPoint *offset;
    QSize _oldSize;
};

#endif // WIDGET_H
