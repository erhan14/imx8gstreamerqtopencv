#include "scaledview.h"

#include <QPainter>
#include <QPaintEvent>
#include <iostream>

ScaledView::ScaledView(QWidget *parent)
    : QOpenGLWidget(parent), pixmap(NULL), qimg(NULL) //QGLFormat(QGL::SampleBuffers),
{}

void ScaledView::setPixmap(const QPixmap &p)
{
    pixmap = &p;
    //resizeEvent(0);
    update();
}

void ScaledView::setQImage(const QImage &img)
{
    qimg = &img;
    update();
}

void ScaledView::resizeEvent(QResizeEvent *ev)
{
    if (!pixmap)
        return;

    // determine scale of correct aspect-ratio
    float src_aspect = pixmap->width()/(float)pixmap->height();
    float dest_aspect = width()/(float)height();
    float w;	// new width
    if (src_aspect > dest_aspect)
        w = width() - 1;
    else
        w = height()*src_aspect - 1;

    scale = w/pixmap->width();
    scaler = QTransform().scale(scale, scale);
    scalerI = scaler.inverted();
}

void ScaledView::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    if (!qimg) {
        qDebug("no pix\n");
        painter.fillRect(this->rect(), QBrush(Qt::gray, Qt::BDiagPattern));
        return;
    }
/*
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.setWorldTransform(scaler);
    QRect damaged = scalerI.mapRect(ev->rect());
    painter.drawPixmap(damaged, *pixmap, damaged);
    */
    painter.drawImage(10, 10, *qimg);
}



void ScaledView::cursorAction(QMouseEvent *ev, bool click)
{
}
