#ifndef SCALEDVIEW_H
#define SCALEDVIEW_H

#include <QOpenGLWidget>
#include "GLES2/gl2.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QGLWidget>
#include <QImage>

class ScaledView : public QOpenGLWidget
{
    Q_OBJECT
public:
    ScaledView(QWidget *parent = 0);
    void setImage(const QImage& image);
    void setPixmap(const QPixmap& image);

protected:
    void paintEvent(QPaintEvent*);

private:
    QImage img;
    QPixmap pmap;
};

#endif // SCALEDVIEW_H
