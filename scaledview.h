#ifndef SCALEDVIEW_H
#define SCALEDVIEW_H

#include <QOpenGLWidget>
//#include "GLES2/gl2.h"
#include <opencv2/opencv.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/imgproc/types_c.h>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QGLWidget>
#include <QImage>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QReadWriteLock>
#include <QQueue>
#include <gst/gst.h>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class ScaledView : public QOpenGLWidget, public QOpenGLFunctions

{
    Q_OBJECT
public:
    ScaledView(QWidget *parent = 0);
    //void setImage(const QImage& image);
    void setPixmap(const QPixmap& image);
    //void setData(const cv::Mat& image);
    ~ScaledView();

    unsigned int			frames;				/** Total amount of rendered frames. */
        unsigned int			painted;			/** Amount of calls to painGL() method. Increases every second, or if contrast/brightness changes or if new image is received. */


public slots:
    void setImage(const QImage& image);
    void setData(const cv::Mat& image);
protected:
    //void paintEvent(QPaintEvent*);
    /** OpenGL initialization. */
    virtual void	initializeGL();
    /** Paint OpenGL content. */
    virtual void	paintGL();
    /** Called whenever widget is resized. */
    virtual void	resizeGL(
            int			width,
            int			height);

    bool					prepared;

        int						remHeight;			/** Height of image. [pixel] */
        GLfloat					invertY;			/** Image is flipped horizontally, if non-zero. */
        GLfloat					contrastMult;		/** Contrast multiplier. [0.0..65535.0] */
        GLfloat					brightnessAdd;		/** Brightness summands. [-256.0..256.0] */
        GLfloat					deinterlace[3];		/** De-interlacing information for shader
                                                        0 .. de-interlacing offset [pixel/texture size]
                                                        1 .. texture size [pixel]
                                                        2 .. invert y, if 1 */
        GLuint id_y;
        GLuint *pbo;

private:
    QQueue<QImage> queue;
    QImage img;
    QPixmap pmap;
    cv::Mat data;

    GLfloat					mvpMatrix[16];		/** Model-view-projection matrix. */

    QOpenGLShaderProgram	program;						/** Shader program. */
    QOpenGLShader			* vertShader;					/** Vertex shader. */
    QOpenGLShader			* fragShader;					/** Fragment shader. */
    QMatrix4x4 pMatrix;
    QVector<QVector3D> vertices;
    QVector<QVector2D> textureCoordinates;

    int						vertPosAttrId;					/** Vertex shader attribute id of "vertPos". */
    int						vertTexCoordAttrId;				/** Vertex shader attribute id of "vertTexCoord". */
    int						vertModelViewAttrId;			/** Vertex shader attribute id of "vertModelView". */
    int						texSamplerId;					/** Fragment shader uniform id of all texture samplers. */
    GLuint					signalTextureId;				/** Texture ID per input signal. */
    int						contrastUniId;					/** Fragment shader uniform id of contrast vector. */
    int						brightnessUniId;				/** Fragment shader uniform id of brightness vector. */
    int						deinterlaceUniId;				/** Fragment shader uniform id of de-interlace values. */

    bool initialCall = true;
    volatile bool processing = false;
    bool m_core;
    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;
    QPoint m_lastPos;
    //Logo m_logo;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_logoVbo;
    QOpenGLShaderProgram *m_program = nullptr;
    int m_projMatrixLoc = 0;
    int m_mvMatrixLoc = 0;
    int m_normalMatrixLoc = 0;
    int m_lightPosLoc = 0;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    static bool m_transparent;
    QReadWriteLock mutex;

};

#endif // SCALEDVIEW_H
