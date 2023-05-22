#include <QPainter>
#include "headers/plotwidget.h"
#include <QWheelEvent>

PlotWidget::PlotWidget(const std::map<float, float>* historique, QWidget *parent)
    : QWidget(parent), historique(historique), scaleFactor(1.0), xOffset(0), yOffset(0)
{
    setMinimumSize(400, 300); // Set a minimum size for the plot widget
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void PlotWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Determine the size of the plot widget
    int width = size().width();
    int height = size().height();

    // Calculate the plot area dimensions
    int margin = 50;
    int plotWidth = width - 2 * margin;
    int plotHeight = height - 2 * margin;

    // Apply scaling and offset to the plot
    int scaledPlotWidth = static_cast<int>(plotWidth * scaleFactor);
    int scaledPlotHeight = static_cast<int>(plotHeight * scaleFactor);
    int scaledXOffset = static_cast<int>(xOffset * scaleFactor);
    int scaledYOffset = static_cast<int>(yOffset * scaleFactor);

    // Draw the coordinate axes
    painter.drawLine(margin + scaledXOffset, margin + scaledYOffset,
                     margin + scaledXOffset, margin + scaledYOffset + scaledPlotHeight); // Y-axis
    painter.drawLine(margin + scaledXOffset, margin + scaledYOffset + scaledPlotHeight,
                     margin + scaledXOffset + scaledPlotWidth, margin + scaledYOffset + scaledPlotHeight); // X-axis

    // Retrieve the min and max values from the historique map
    auto minMax = std::minmax_element(historique->begin(), historique->end(),
                                      [](const std::pair<float, float>& p1, const std::pair<float, float>& p2) {
                                          return p1.second < p2.second;
                                      });
    float minY = minMax.first->second;
    float maxY = minMax.second->second;

    // Draw the plot points
    float scaleX = static_cast<float>(scaledPlotWidth) / (historique->rbegin()->first - historique->begin()->first);
    float scaleY = static_cast<float>(scaledPlotHeight) / (maxY - minY);
    for (const auto& point : *historique) {
        int x = margin + scaledXOffset + static_cast<int>((point.first - historique->begin()->first) * scaleX);
        int y = margin + scaledYOffset + scaledPlotHeight - static_cast<int>((point.second - minY) * scaleY);
        painter.drawEllipse(QPoint(x, y), 2, 2);
    }

    // Draw axis graduations and labels
    QFont font;
    font.setPointSize(8);
    painter.setFont(font);

    // X-axis graduations
    int numGraduationsX = 5; // Number of graduations on the X-axis
    float xStep = (historique->rbegin()->first - historique->begin()->first) / numGraduationsX;
    float x = historique->begin()->first;
    for (int i = 0; i <= numGraduationsX; i++) {
        int xCoord = margin + scaledXOffset + static_cast<int>((x - historique->begin()->first) * scaleX);
        painter.drawLine(xCoord, margin + scaledYOffset + scaledPlotHeight, xCoord, margin + scaledYOffset + scaledPlotHeight + 5);
        painter.drawText(xCoord - 20, margin + scaledYOffset + scaledPlotHeight + 15, QString::number(x));
        x += xStep;
    }

    // Y-axis graduations
    int numGraduationsY = 5; // Number of graduations on the Y-axis
    float yStep = (maxY - minY) / numGraduationsY;
    float y = minY;
    for (int i = 0; i <= numGraduationsY; i++) {
        int yCoord = margin + scaledYOffset + scaledPlotHeight - static_cast<int>((y - minY) * scaleY);
        painter.drawLine(margin + scaledXOffset - 5, yCoord, margin + scaledXOffset, yCoord);
        painter.drawText(margin + scaledXOffset - 45, yCoord + 5, QString::number(y));
        y += yStep;
    }
}

void PlotWidget::wheelEvent(QWheelEvent *event)
{
    // Scale the plot based on the wheel event
    int delta = event->angleDelta().y();
    double scaleFactorDelta = 1.1; // Scale factor for zoom in/out

    if (delta > 0) {
        // Zoom in
        scaleFactor *= scaleFactorDelta;
    } else {
        // Zoom out
        scaleFactor /= scaleFactorDelta;
    }

    update();
}

void PlotWidget::mousePressEvent(QMouseEvent *event)
{
    // Store the initial position for scrolling
    if (event->button() == Qt::LeftButton) {
        lastMousePos = event->pos();
    }
}

void PlotWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Scroll the plot based on the mouse movement
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - lastMousePos;
        xOffset += delta.x();
        yOffset += delta.y();
        lastMousePos = event->pos();
        update();
    }
}
