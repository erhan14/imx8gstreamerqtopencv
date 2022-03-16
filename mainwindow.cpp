#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>
#include <mutex>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QElapsedTimer>
using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //glWidget = new ScaledView(ui->centralwidget);

//    th1 = std::async([&]{
//        cam1.set(cv::CAP_PROP_BUFFERSIZE, 200);
//        if(cam1.open(STREAM1,cv::CAP_GSTREAMER, {CAP_PROP_HW_ACCELERATION, VIDEO_ACCELERATION_NONE} )){
//            qDebug("Cam1 is successfully opened!\n");

//            eventForCameraChoose(Cameras::Cam1);
//            return true;
//        }else{
//            qDebug("Cam1 is failed open!\n");
//        }
//        return false;
//    });


    //ui->videoLabel->setVisible(true);


//    if(th1.wait_for(std::chrono::milliseconds(500)) == std::future_status::ready){
//        th1.get();
//        qDebug("Cam1 ok!\n");

//    }

    /***
     * Gstreamer version
     * */
    fps = -1;
    QUrl url = QUrl::fromUserInput("rtsp://admin:Fotoniks2010@192.168.1.176:554/ch3/main/av_stream");
    qmlplayer_init(url);

    th1 = std::async([&]{
        while(fps==-1);
        qDebug("Link ready!\n");
        return true;
    });
    if(th1.wait_for(std::chrono::milliseconds(15000)) == std::future_status::ready){
        th1.get();
        qDebug("Cam1 link ok!\n");
    }

    th = std::async(&MainWindow::videoCaptureThread,this);
}

MainWindow::~MainWindow()
{
    if(th.valid()){
        th.get();
    }else{
        std::cout << "th is not valid." << std::endl;
    }
    delete ui;
}

/* pad added handler */
void MainWindow::pad_added_handler1 (GstElement *src, GstPad *new_pad, CustomData *data) {
GstPad *sink_pad = gst_element_get_static_pad (data->rtppay, "sink");
GstPadLinkReturn ret;
GstCaps *new_pad_caps = NULL;
GstStructure *new_pad_struct = NULL;
const gchar *new_pad_type = NULL;

g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));

/* Check the new pad's name */
if (!g_str_has_prefix (GST_PAD_NAME (new_pad), "recv_rtp_src_")) {
    g_print ("  It is not the right pad.  Need recv_rtp_src_. Ignoring.\n");
    goto exit;
}

/* If our converter is already linked, we have nothing to do here */
if (gst_pad_is_linked (sink_pad)) {
    g_print (" Sink pad from %s already linked. Ignoring.\n", GST_ELEMENT_NAME (src));
    goto exit;
}

/* Check the new pad's type */
new_pad_caps = gst_pad_get_current_caps (new_pad);
new_pad_struct = gst_caps_get_structure (new_pad_caps, 0);
new_pad_type = gst_structure_get_name (new_pad_struct);


/* Attempt the link */
ret = gst_pad_link (new_pad, sink_pad);
if (GST_PAD_LINK_FAILED (ret)) {
    g_print ("  Type is '%s' but link failed.\n", new_pad_type);
} else {
    g_print ("  Link succeeded (type '%s').\n", new_pad_type);

fps= 0;

//    GstPad *pad = gst_element_get_static_pad(new_pad , "source");
//    if(!GST_IS_PAD (pad)){
//        qDebug("no pad\n");
//    }
//    int i = gst_pad_add_probe (new_pad, GST_PAD_PROBE_TYPE_BUFFER,
//         ((GstPadProbeCallback) cb_have_data), NULL, NULL);
//    qDebug("gst_pad_add_probe %d\n", i);
//    gst_object_unref (pad);

}

exit:
/* Unreference the new pad's caps, if we got them */
if (new_pad_caps != NULL)
    gst_caps_unref (new_pad_caps);

/* Unreference the sink pad */
gst_object_unref (sink_pad);
}

GstPadProbeReturn MainWindow::cb_have_data (GstPad          *pad,
              GstPadProbeInfo *info,
              gpointer         user_data)
{
  gint x, y;
  GstMapInfo map;
  guint16 *ptr, t;
  GstBuffer *buffer;

  if(tim==nullptr)
      qDebug("tim null\n");
  if(!tim->isValid())
      tim->start();
  buffer = GST_PAD_PROBE_INFO_BUFFER (info);

  buffer = gst_buffer_make_writable (buffer);

  /* Making a buffer writable can fail (for example if it
   * cannot be copied and is used more than once)
   */
  if (buffer == NULL) {
      qDebug("buffer null\n");
      return GST_PAD_PROBE_OK;
  }

  qDebug("cb_have_data\n");
  /* Mapping a buffer can fail (non-writable) */
  if (gst_buffer_map (buffer, &map, GST_MAP_WRITE)) {
    ptr = (guint16 *) map.data;
    /* invert data */
    for (y = 0; y < 288; y++) {
      for (x = 0; x < 384 / 2; x++) {
        t = ptr[384 - 1 - x];
        ptr[384 - 1 - x] = ptr[x];
        ptr[x] = t;
      }
      ptr += 384;
    }
    gst_buffer_unmap (buffer, &map);
  }

  GST_PAD_PROBE_INFO_DATA (info) = buffer;

  if(tim->hasExpired(1000)) {
      qDebug("FPS: %d\n", fps);
      fps = 0;
      tim->restart();
  }
  return GST_PAD_PROBE_OK;
}

void MainWindow::videoCaptureThread()
{
    cv::VideoCapture *cap=nullptr;
    //while(true){
        flag=false;
        switch (currentCam.load()) {
        case Cameras::Cam1:
            cap = &cam1;
            break;
        case Cameras::Cam2:
            cap = &cam2;
            break;
        case Cameras::Cam3:
            cap = &cam3;
            break;
        case Cameras::Cam4:
            cap = &cam4;
            break;
        case Cameras::AllCam:
            break;
        }


        loopForDiscreteVideo(cap);

    //}
}

void MainWindow::eventForCameraChoose(Cameras camera){
    qDebug("Camera chosen\n");
    flag=true;
    currentCam = camera;
}

void storeStreamToTexture(GLuint texture, cv::Mat* data){
    //Bind the texture we want to render to.
    glBindTexture(GL_TEXTURE_2D, texture);
    //we flip the Mat to start reading from the beginning.
    cv::flip(*data, *data, 0);
    //Store mat data to our texture.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data->cols, data->rows, 0, GL_RGB, GL_UNSIGNED_BYTE, data->data);
}

int
MainWindow::qmlplayer_init(const QUrl url)
{

    /****************************Pipeline*****************************************
                    => audioconvert => audioresample => volume => alsasink
    uridecodebin <
                    => videoconvert => glupload => qmlglsink => videoItem(Qt)

 gst-launch-1.0 rtspsrc location=rtsp://admin:adminFTX1@192.168.1.66:554/ch5/main/av_stream
caps="application/x-rctp, media=(string)video"
! rtph265depay
! h265parse
! queue
! vpudec
! videoconvert
! autovideosink sync=false
    *****************************************************************************/
    GstPad *pad;
    //gst_debug_set_default_threshold(GST_LEVEL_DEBUG);
    data.pipeline = gst_pipeline_new(nullptr);
    qDebug("pipeline created\n");
    data.source = gst_element_factory_make ("rtspsrc","source");
    g_object_set (G_OBJECT (data.source), "buffer-mode", 0, NULL);
    g_object_set (G_OBJECT (data.source), "do-retransmission", FALSE, NULL);
    g_object_set (G_OBJECT (data.source), "latency", 0, NULL);
    qDebug("rtspsrc init\n");

    data.rtppay = gst_element_factory_make( "rtph265depay", "depayl");
    qDebug("rtph265depay init\n");
    data.parse = gst_element_factory_make("h265parse","parse");
    qDebug("h265parse init\n");
    data.filter1 = gst_element_factory_make("capsfilter","filter");
    qDebug("capsfilter init\n");
    data.queue = gst_element_factory_make("queue", nullptr);
    data.decode = gst_element_factory_make ("vpudec","decode");
    //data.decode = gst_element_factory_make ("h264dec", "decode");
    qDebug("vpudec init\n");
    data.videoconvert = gst_element_factory_make("videoconvert", nullptr);
    qDebug("videoconvert init\n");
    //data.glupload = gst_element_factory_make("glupload", nullptr);
    //qDebug("glupload init\n");
    //data.qmlglsink = gst_element_factory_make("qmlglsink", "sink");
    //data.xraw = gst_element_factory_make("video/x-raw,format=RGB16", nullptr);
    data.appsink = gst_element_factory_make("appsink", "sink");
    //g_object_set (G_OBJECT (data.appsink), "caps", "video/x-raw,format=RGB16", NULL);
    qDebug("qmlglsink init\n");

    //g_object_set (G_OBJECT (data.qmlglsink), "sync", FALSE, NULL);
    g_object_set (G_OBJECT (data.appsink), "sync", FALSE, NULL);

    g_object_set(GST_OBJECT(data.source),"location", url.toEncoded().data(), NULL);
    qDebug("location set %s\n", url.toEncoded().data());

    //application/x-rtp
    //GstCaps *filtercaps = gst_caps_from_string("application/x-rctp, media=(string)video");
    GstCaps *filtercaps = gst_caps_from_string("application/x-rtp");
    g_object_set (G_OBJECT (data.filter1), "caps", filtercaps, NULL);

    gst_caps_unref(filtercaps);
    qDebug("caps init\n");

    gst_app_sink_set_drop(GST_APP_SINK(data.appsink), true);
    gst_app_sink_set_max_buffers(GST_APP_SINK(data.appsink), 1);

    gst_bin_add_many (GST_BIN (data.pipeline), data.source
            ,data.rtppay, data.parse, data.filter1, data.queue, data.decode,
             data.videoconvert, /*data.glupload, data.qmlglsink data.xraw,*/ data.appsink
            ,NULL);

    if (!gst_element_link_many(data.rtppay, data.parse, data.queue, data.decode, data.videoconvert, /*data.glupload, data.qmlglsink data.xraw,*/ data.appsink, NULL)) {
            g_printerr("video elements could not be linked.\n");
            gst_object_unref(data.pipeline);
            return -1;
        }

    g_signal_connect(data.source, "pad-added", G_CALLBACK(pad_added_handler1), &data);

    qDebug("all complete\n");

    /**** Wait for playing mode ****/
    play = new SetPlaying(this->data.pipeline);
    play->run();
    return 0;
}

static string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

void MainWindow::loopForDiscreteVideo(cv::VideoCapture *cap)
{
    qDebug("loopForDiscreteVideo");
    QElapsedTimer *t = new QElapsedTimer();
    int fps = 0;

    t->start();

//    int once = 0;

    while (true) {
        GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(data.appsink));
          if (sample == NULL) {
            qDebug("%s: Failed to get new sample", __FUNCTION__);
            return;
          }

          // FIXME: zero copy?
          /*GstCaps *caps = gst_sample_get_caps(sample);
          GstStructure *structure = gst_caps_get_structure(caps, 0);
          const int width = g_value_get_int(gst_structure_get_value(structure, "width"));
          const int height = g_value_get_int(gst_structure_get_value(structure, "height"));*/
          const int width = 1920;
          const int height = 1080;
          GstBuffer *buffer = gst_sample_get_buffer(sample);
          GstMapInfo map;

          if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {

              /*GstVideoInfo * info = gst_video_info_new();
              gst_video_info_from_caps(info, caps);
              if(once==0)
                qDebug("Image %d x %d %d",info->width, info->height, info->finfo->format);
              once = 1;*/


              // yuv to bgr
              cv::Mat mYUV(height + height/2, width, CV_8UC1, (void*) map.data);
              cv::Mat frame(height, width, CV_8UC3);
              cvtColor(mYUV, frame, CV_YUV2BGR_I420, 3);

              //qDebug("type %s", type2str(frame.type()).c_str());
//             cv::Mat frame(cv::Size(width, height), CV_8UC3, (char *)map.data, cv::Mat::AUTO_STEP);

              QImage dest(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
//              dest.bits();
              ui->glWidget->setPixmap(QPixmap::fromImage(dest));


              //ui->videoLabel->raise();
              //ui->videoLabel->setPixmap(QPixmap::fromImage(dest));
                gst_buffer_unmap(buffer, &map);
          } else {
            qWarning("Error with gst_buffer_map");
          }
          gst_sample_unref(sample);
          fps++;
          if(t->hasExpired(1000)) {
              qDebug("FPS: %d\n", fps);
              fps = 0;
              t->restart();
          }

//        static cv::Mat frame;
//        static cv::Mat frame2xZoom;
//        if(cap->read(frame) && !frame.empty()){

//            if(flag2xZoom){
//                frame2xZoom = make2xZoom(frame);
//                ui->videoLabel->setPixmap(QPixmap::fromImage(QImage(frame2xZoom.data, frame2xZoom.cols, frame2xZoom.rows, frame2xZoom.step, QImage::Format_BGR888).scaled(ui->videoLabel->width(),ui->videoLabel->height())));
//            }else{
//                //cv::imshow("rtsp_stream", frame);

//                cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
//                ui->glWidget->setQImage(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888));

//                switch(cv::waitKey(1)){
//                case 0x1b: //ESC key
//                    qDebug("Closing stream.\n");
//                    break;
//                }


//                //qDebug("1\n");
//                //ui->glWidget->update();
//                //ui->videoLabel->setPixmap(QPixmap::fromImage(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888).scaled(ui->videoLabel->width(),ui->videoLabel->height())));
//            }
//            fps++;
//            if(t->hasExpired(1000)) {
//                qDebug("FPS: %d\n", fps);
//                fps = 0;
//                t->restart();
//            }
//        }
//        else
//        {
//            //qDebug("empty\n");
//            //ui->videoLabel->setPixmap(QPixmap::fromImage(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888).scaled(ui->videoLabel->width(),ui->videoLabel->height())));
//        }

        if(flag){
            qDebug("exit\n");
            break;
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //QCoreApplication::processEvents();
    }
}

