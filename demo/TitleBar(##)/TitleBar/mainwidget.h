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

public slots:
    void dealCloWindow();
    void dealMaxWindow();
    void dealUnmaxWindow();
    void dealMinWindow();
    void dealMovWindow(QPoint);


protected:
    void resizeEvent(QResizeEvent *);
    void moveEvent(QMoveEvent *);

private:
    Ui::MainWidget *ui;
    QSize _oldSize;
    QSize _newSize;
    QPoint _oldPos;
    QPoint _newPos;
};

#endif // MAINWIDGET_H
