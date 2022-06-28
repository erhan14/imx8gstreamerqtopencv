#include "scaledview.h"

#include <QPainter>
#include <QPaintEvent>
#include <iostream>

ScaledView::ScaledView(QWidget *parent)
    : QOpenGLWidget(parent), img(NULL) /*, QGLFormat(QGL::SampleBuffers)*/
    ,prepared(false)
{
m_core = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
}

void ScaledView::setImage(const QImage& image)
{
    qDebug("m1 %dx%d\n", image.width(), image.height());
    //mutex.lockForWrite();
    img = QGLWidget::convertToGLFormat(image);
    //queue.enqueue(image);
    //mutex.unlock();
    //qDebug("m2\n");
    prepared = true;
    update();
}

void ScaledView::setPixmap(const QPixmap& image)
{
    pmap = image;
    prepared = true;
    update();
}

void ScaledView::setData(const cv::Mat& image)
{
    //qDebug("m1\n");
    if(!processing) {
        data = image.clone();
        //cv::cvtColor(data, data, cv::COLOR_RGBA2BGR);
        cv::flip(data, data, 0);
        prepared = true;
        //qDebug("m2\n");
        update();
    }
}

ScaledView::~ScaledView() {
    if (m_program == nullptr)
        return;
    makeCurrent();
    m_logoVbo.destroy();
    delete m_program;
    m_program = nullptr;
    doneCurrent();
}

/*
void ScaledView::paintEvent(QPaintEvent*)
{
    QPainter p(this);
     //Set the painter to use a smooth scaling algorithm.
    p.setRenderHint(QPainter::SmoothPixmapTransform, 1);

    //p.setClipping(true);
    //p.setClipRect(QRect(0, 0, size().width(), size().height()));

    //qDebug("Try update\n");
    p.drawImage(this->rect(), img);
    //qDebug("Ok update\n");
    //p.drawPixmap(this->rect(), pmap);
    //p.end();

    //glBindTexture(GL_TEXTURE_2D, texture);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.width(), frame.height(),0, GL_BGRA, GL_UNSIGNED_BYTE, frame.bits());
}*/

static const char *vertexShaderSource = //pos
        "attribute vec3 position;\n" //verilenler
        "attribute vec2 texcoord;\n"
        "varying vec2 uv;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(position, 1.0);\n"
        "    uv = texcoord;\n"
        "}\n";

static const char *fragmentShaderSource = //color
        "uniform highp sampler2D image;\n"
        "varying lowp vec2 uv;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = texture2D(image, uv);\n"
        "}\n";

const void check(int i) {
GLenum err;
while((err = glGetError()) != GL_NO_ERROR)
{
  // Process/log the error.
    qDebug("glerror:%d, %d", i, err);
}
}

void ScaledView::initializeGL()
{
    initializeOpenGLFunctions();
    qDebug("1\n");
    //glEnable(GL_TEXTURE_2D);
    glClearColor(0.85, 0.85, 0.85, 1.0);
    //glClearColor(0.3,0,0,1);
    //glEnable(GL_DEPTH_TEST);

    if(!program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource))
        exit(0);
    qDebug("vshader %s\n", program.log().toLatin1().constData());
    if(!program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource))
        exit(0);
    qDebug("fshader %s\n",program.log().toLatin1().constData());

    program.bindAttributeLocation("position", 0);
    program.bindAttributeLocation("texcoord", 1);

    if(!program.link()) { qDebug("link err %s\n", program.log().toLatin1().constData()); exit(0);}
    //if(!program.bind()) { qDebug("bind err %s\n", program.log()); exit(0);}

    vertices << QVector3D(-1, -1, 0) << QVector3D(-1, 1, 0) << QVector3D(1, 1, 0) << QVector3D(1, 1, 0) << QVector3D(1, -1, 0) << QVector3D(-1, -1, 0);

    textureCoordinates << QVector2D(0, 0) << QVector2D(0, 1) << QVector2D(1, 1) // Front
                       << QVector2D(1, 1) << QVector2D(1, 0) << QVector2D(0, 0);

    glGenTextures(1, &id_y); // Obtain an id for the texture
    check(3);

    program.setAttributeArray("position", vertices.constData());
    program.enableAttributeArray("position");
    check(9);
    program.setUniformValue("image", 0);
    //glUniform1i(program.uniformLocation("image"), 0);
    check(10);
    program.setAttributeArray("texcoord", textureCoordinates.constData());
    program.enableAttributeArray("texcoord");
    check(11);
    program.bind();
    check(12);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, id_y);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}

void ScaledView::paintGL()
{
    if(initialCall) {
        //qDebug("2\n");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        check(1);
        //glEnable(GL_TEXTURE_2D);
        //check(2);


    //glActiveTexture(GL_TEXTURE0);
    //check(14);
    glBindTexture(GL_TEXTURE_2D, id_y); // Set as the current texture
    check(4);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    check(5);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    check(6);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    check(7);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    check(8);
    }

        if(prepared /*&& !queue.isEmpty()*/) {
            //qDebug("4 %d %d\n", data.cols, data.rows);
            //qDebug("m3\n");
            //mutex.lockForWrite();
            //QImage tex = QGLWidget::convertToGLFormat(img);
            //QImage f = queue.dequeue();
            //QImage tex = QGLWidget::convertToGLFormat(f);
            //mutex.unlock();

            //qDebug("4 %d %d\n", tex.width(), tex.height());
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

            //cv::flip(data, data, 0);
            //qDebug("m4\n");
            processing = true;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.cols, data.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.ptr());
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
            processing=false;
            //qDebug("m5\n");
            check(13);

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
            //glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_SHORT, 0);
            check(15);
            //qDebug("m4\n");
            initialCall = false;
        }

        //glActiveTexture ( GL_TEXTURE0 );
        //check(14);
        //glBindTexture ( GL_TEXTURE_2D, id_y );



        /*program.disableAttributeArray("position");
        check(16);
        program.disableAttributeArray("texcoord");
        check(17);
        program.release();
        check(18);
        */
//qDebug("3\n");
}

/** Called whenever widget is resized. */
void
ScaledView::resizeGL(
        int		width,
        int		height) {

    glViewport(0, 0, width, height);
}
