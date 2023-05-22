#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class PlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlotWidget(const std::map<float, float>* historique, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    const std::map<float, float>* historique;
    double scaleFactor;
    int xOffset;
    int yOffset;
    QPoint lastMousePos;
};

#endif // PLOTWIDGET_H
