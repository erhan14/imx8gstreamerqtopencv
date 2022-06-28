#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/types_c.h>
#include <atomic>
#include <future>
#include "scaledview.h"
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/video/video-info.h>
#include <QUrl>
#include "setplaying.h"
#include <QElapsedTimer>
#include "processingthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    typedef struct _CustomData {
        GstElement *pipeline;
        GstElement *decode;
        GstElement *videoconvert;
        GstElement *glupload;
        GstElement *qmlglsink;
        GstElement *audioconvert;
        GstElement *audioresample;
        GstElement *volume;
        GstElement *alsasink;
        GstElement *rtppay;
        GstElement *parse;
        GstElement *filter1;
        GstElement *source;
        GstElement *queue;
        GstElement *queue2;
        GstElement *xraw;
        GstElement *xraw2;
        GstElement *appsink;
    } CustomData;

    CustomData data;
    SetPlaying* play;
    gint64 duration;
    static QElapsedTimer *tim;
    static volatile int fps;
    ProcessingThread * processingThread;
private slots:

signals:
    void incoming(QImage image);
    void incomingMat(const cv::Mat& image);

private:
    std::future<void> th;
    std::future<bool> th1 ;
    std::future<bool> th2 ;
    std::future<bool> th3 ;
    std::future<bool> th4 ;
    //gst-launch-1.0 v4l2src device=/dev/video0 ! videoscale ! videoconvert ! x264enc bitrate=1024 speed-preset=superfast qp-min=30 tune=zerolatency  ! mpegtsmux ! rndbuffersize min=188 max=188 ! udpsink host=127.0.0.1 port=5001
    //gst-launch-1.0 videotestsrc pattern=snow ! videoscale ! videoconvert ! x264enc bitrate=1024 speed-preset=superfast qp-min=30 tune=zerolatency  ! mpegtsmux ! rndbuffersize min=188 max=188 ! udpsink host=127.0.0.1 port=5002
    //constexpr static const char *STREAM1="uridecodebin uri=rtsp://admin:adminFTX1@192.168.1.66:554/ch5/main/av_stream ! videoconvert ! queue ! appsink sync=false";

    //constexpr static const char *STREAM1="rtspsrc buffer-mode=0 do-retransmission=false latency=0 location=rtsp://192.168.1.250:8554/sample caps=\"application/x-rtp\" ! rtph264depay! h264parse ! queue  ! vpudec ! videoconvert ! appsink sync=false";
    //constexpr static const char *STREAM1="rtspsrc latency=0 location=rtsp://admin:Fotoniks2010@192.168.1.176:554/ch3/main/av_stream ! rtph265depay ! h265parse ! queue  ! vpudec ! videoconvert ! appsink sync=false";
    constexpr static const char *STREAM1="rtspsrc latency=0 location=rtsp://admin:Fotoniks2010@192.168.1.64:554/ch1/main/av_stream ! rtph264depay ! h264parse ! queue  ! vpudec ! videoconvert ! appsink sync=false";

    //rtsp://admin:Fotoniks2010@192.168.1.176:554/ch3/main/av_stream
#define IMX8_
#ifdef IMX8_
    // rtsp://192.168.1.3:8554/sample
    constexpr static const char *STREAM2="rtspsrc buffer-mode=0 do-retransmission=false latency=0 location=rtsp://192.168.1.3:8554/sample caps=\"application/x-rtp\" ! rtph264depay! h264parse ! queue  ! vpudec ! videoconvert ! appsink sync=false";
    //constexpr static const char *STREAM2="rtspsrc location=rtsp://admin:Fotoniks2010@192.168.1.64:554/ch1/main/av_stream caps=\"application/x-rctp, media=(string)video\" ! rtph264depay! h264parse ! queue  ! vpudec ! videoconvert ! appsink sync=false";
#else
    constexpr static const char *STREAM2="rtspsrc location=rtsp://admin:adminFTX1@192.168.1.66:554/ch5/main/av_stream caps=\"application/x-rtp, media=(string)video\" latency = 0 ! rtph265depay ! queue ! avdec_h265 ! videoconvert ! appsink sync=false";
#endif
    constexpr static const char *STREAM3="rtsp://admin:adminFTX1@192.168.1.66:554/ch5/main/av_stream"; //Opencv capture Latency olmasi normal
    constexpr static const char *STREAM4="";//"uridecodebin uri=udp://127.0.0.1:5003 ! videoconvert ! appsink sync=false";
    enum class VideoLabel{
        DiscreteVideo,
        Small4Videos
    };

    enum class Cameras{
        Cam1,
        Cam2,
        Cam3,
        Cam4,
        AllCam
    };
    volatile std::atomic<Cameras> currentCam{Cameras::Cam3};
    volatile std::atomic<bool> flag{false};
    volatile std::atomic<bool> flag2xZoom{false};

    void eventForCameraChoose(Cameras);
    void loopForDiscreteVideo(cv::VideoCapture *);
    void emitSignals(const cv::Mat&);
    void videoCaptureThread();//   gst-launch-1.0 v4l2src device=/dev/video0 ! videoscale ! videoconvert ! x264enc bitrate=1024 speed-preset=superfast qp-min=30 tune=zerolatency  ! mpegtsmux ! rndbuffersize min=188 max=188 ! udpsink host=127.0.0.1 port=5001
    cv::VideoCapture cam1{};//=cv::VideoCapture("gst-launch-1.0 uridecodebin uri=udp://127.0.0.1:5001 ! videoconvert ! appsink sync=false",cv::CAP_GSTREAMER);
    cv::VideoCapture cam2{};//=cv::VideoCapture("gst-launch-1.0 uridecodebin uri=udp://127.0.0.1:5002 ! videoconvert ! appsink sync=false",cv::CAP_GSTREAMER);
    cv::VideoCapture cam3{};//=cv::VideoCapture("gst-launch-1.0 uridecodebin uri=udp://127.0.0.1:5003 ! videoconvert ! appsink sync=false",cv::CAP_GSTREAMER);
    cv::VideoCapture cam4{};//=cv::VideoCapture("gst-launch-1.0 uridecodebin uri=udp://127.0.0.1:5004 ! videoconvert ! appsink sync=false",cv::CAP_GSTREAMER);

    Ui::MainWindow *ui;

     int qmlplayer_init(const QUrl url);
     static void pad_added_handler1 (GstElement *src, GstPad *new_pad, CustomData *data);
     static GstPadProbeReturn cb_have_data (GstPad *pad, GstPadProbeInfo *info, gpointer user_data);
};
#endif // MAINWINDOW_H
