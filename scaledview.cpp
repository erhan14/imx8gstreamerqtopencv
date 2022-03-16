#include "scaledview.h"

#include <QPainter>
#include <QPaintEvent>
#include <iostream>

ScaledView::ScaledView(QWidget *parent)
    : QOpenGLWidget(parent), img(NULL) /*, QGLFormat(QGL::SampleBuffers)*/
{

}

void ScaledView::setImage(const QImage& image)
{
    img = image;
    update();
}

void ScaledView::setPixmap(const QPixmap& image)
{
    pmap = image;
    update();
}

void ScaledView::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    //Set the painter to use a smooth scaling algorithm.
    p.setRenderHint(QPainter::SmoothPixmapTransform, 1);

    //p.drawImage(this->rect(), img);
    p.drawPixmap(this->rect(), pmap);

}
