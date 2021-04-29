#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_closeButton_clicked();
    void on_maxButton_clicked();
    void on_minButton_clicked();

private:
    Ui::Widget *ui;
    QPoint *pos_offset;
};

#endif // WIDGET_H
