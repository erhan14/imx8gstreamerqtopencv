#include "mainwindow.h"

#include <QApplication>

#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>

using namespace std;


#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
using namespace cv;

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
//    //    // The sink caps for the 'rtpjpegdepay' need to match the src caps of the 'rtpjpegpay' of the sender pipeline
//    //       // Added 'videoconvert' at the end to convert the images into proper format for appsink, without
//    //       // 'videoconvert' the receiver will not read the frames, even though 'videoconvert' is not present
//    //       // in the original working pipeline
//    //       VideoCapture cap("rtspsrc location=rtsp://admin:adminFTX1@192.168.1.66:554/ch5/main/av_stream latency=0 ! rtph265depay ! vpudec ! appsink sync=false",
//    //               CAP_GSTREAMER);
//    //        std::cout << "gst-launch1.0 rtspsrc location=rtsp://admin:adminFTX1@192.168.1.66:554/ch5/main/av_stream latency=0 ! rtph265depay ! vpudec ! appsink sync=false" <<std::endl;
//    //       if (!cap.isOpened()) {
//    //           cerr <<"VideoCapture not opened"<<endl;
//    //           exit(-1);
//    //       }

//    //       Mat frame{};
//    //       while (true) {


//    //           cap.read(frame);

//    //           imshow("receiver", frame);

//    //           if (waitKey(10) == 27) {
//    //               break;
//    //           }
//    //       }

//    //       return 0;



////    std::string pipe = "rtspsrc location=rtsp://admin:adminFTX1@192.168.1.66:554/ch5/main/av_stream  latency=0 ! rtph265depay ! vpudec ! videoconvert ! appsink";

////    VideoCapture cap(pipe, CAP_GSTREAMER);

////    if (!cap.isOpened()) {
////        cerr <<"VideoCapture not opened"<<endl;
////        exit(-1);
////    }

////    Mat frame;

////    while (true) {
////        cap >> frame;

////        imshow("receiver.png", frame);
////        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
////        waitKey(5);
////    }

////    return 0;



    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

////https://github.com/enthusiasticgeek/gstreamer-rtsp-ssl-example
//#include <string.h>
//#include <math.h>
//#include <gst/gst.h>
//#include <glib.h>
//#include <gio/gio.h>

//static gboolean bus_call (GstBus *bus,GstMessage *msg, gpointer data) {
//    GMainLoop *loop = (GMainLoop *) data;
//    switch (GST_MESSAGE_TYPE (msg)) {
//    case GST_MESSAGE_EOS:
//        g_print ("Stream Ends\n");
//        g_main_loop_quit (loop);
//        break;
//    case GST_MESSAGE_ERROR: {
//        gchar  *debug;
//        GError *error;
//        gst_message_parse_error (msg, &error, &debug);
//        g_free (debug);
//        g_printerr ("Error: %s\n", error->message);
//        g_error_free (error);
//        g_main_loop_quit (loop);
//        break;
//    }
//    case GST_MESSAGE_INFO:{
//        g_print ("!!!!\n");
//        break;
//    }
//    default:
//        break;
//    }
//    return TRUE;
//}


///* Dynamically link */
//static void on_pad_added (GstElement *element, GstPad *pad, gpointer data){
//    GstPad *sinkpad;
//    GstPadLinkReturn ret;
//    GstElement *decoder = (GstElement *) data;
//    /* We can now link this pad with the rtsp-decoder sink pad */
//    g_print ("Dynamic pad created, linking source/demuxer\n");
//    sinkpad = gst_element_get_static_pad (decoder, "sink");

//    /* If our converter is already linked, we have nothing to do here */
//    if (gst_pad_is_linked (sinkpad)) {
//        g_print("*** We are already linked ***\n");
//        gst_object_unref (sinkpad);
//        return;
//    } else {
//        g_print("proceeding to linking ...\n");
//    }
//    ret = gst_pad_link (pad, sinkpad);

//    if (GST_PAD_LINK_FAILED (ret)) {
//        //failed
//        g_print("failed to link dynamically\n");
//    } else {
//        //pass
//        g_print("dynamically link successful\n");
//    }

//    gst_object_unref (sinkpad);
//}

///* Dynamically link */
//static void on_pad_removed (GstElement *element, GstPad *pad, gpointer data){
//    GstPad *sinkpad;
//    GstElement *decoder = (GstElement *) data;
//    /* We can now link this pad with the rtsp-decoder sink pad */
//    g_print ("Dynamic pad created, unlinking source/demuxer\n");
//    sinkpad = gst_element_get_static_pad (decoder, "sink");

//    /* If our converter is already linked, we have nothing to do here */
//    if (gst_pad_is_linked (sinkpad)) {
//        g_print("proceeding to unlinking ...\n");
//    } else {
//        g_print("*** We are already unlinked ***\n");
//        gst_object_unref (sinkpad);
//        return;
//    }
//    auto ret = gst_pad_unlink (pad, sinkpad);

//    if (GST_PAD_LINK_FAILED (ret)) {
//        //failed
//        g_print("failed to unlink dynamically\n");
//    } else {
//        //pass
//        g_print("dynamically unlink successful\n");
//    }
//    gst_object_unref (sinkpad);
//}


//int main (int argc, char *argv[])
//{

//    if(argc!=2){
//      g_print("Please pass the URL of the RTSP stream\ne.g. %s rtsps://127.0.0.1:8554/test\n",argv[0]);
//      exit(1);
//    }

//    GMainLoop *loop;
//    GstBus *bus;
//    GstElement *pipeline;
//    GstElement *rtspsrc;
//    GstElement *rtph264depay;
//    GstElement *h264parse;
//    GstElement *avdec_h264;
//    GstElement *videoqueue0;
//    GstElement *videoconvert;

//    GstElement *video_sink; /* The video sink element which receives XOverlay commands */


//    /* Initializing GStreamer */
//    gst_init (&argc, &argv);
//    loop = g_main_loop_new (NULL, FALSE);

//    //gst-launch-0.10 rtspsrc location=rtsp://<ip> ! decodebin ! ffmpegvideoconvert ! autovideosink
//    //gst-launch -v rtspsrc location="rtsp://<ip> ! rtpmp4vdepay ! mpeg4videoparse ! ffdec_mpeg4 ! ffmpegvideoconvert! autovideosink
//    //gst-launch -v rtspsrc location="rtsp://<ip> ! rtpmp4vdepay ! ffdec_mpeg4 ! ffmpegvideoconvert! autovideosink
//    /* Create Pipe's Elements */
//    pipeline = gst_pipeline_new ("video player");
//    g_assert (pipeline);

//    rtspsrc = gst_element_factory_make ("rtspsrc", "rtspsrc0");
//    g_assert (rtspsrc);


//    //Video elements
//    rtph264depay = gst_element_factory_make ("rtph265depay", "rtph265depay0");
//    g_assert (rtph264depay);
//    h264parse = gst_element_factory_make ("h265parse", "h265parse0");
//    g_assert (h264parse);
//    avdec_h264 = gst_element_factory_make ("vpudec", "avdec_h2540");
//    g_assert (avdec_h264);
//    videoqueue0 = gst_element_factory_make ("queue", "videoqueue0");
//    g_assert (videoqueue0);
//    videoconvert = gst_element_factory_make ("videoconvert", "videoconvert0");
//    g_assert (videoconvert);
//    //Video Sink
//    video_sink = gst_element_factory_make ("autovideosink", "autovideosink0");
//    //video_sink = gst_element_factory_make ("ximagesink", "autovideosink0");
//    g_assert (video_sink);
//    //video
//    g_object_set (G_OBJECT (video_sink), "sync", FALSE, NULL);
//    //g_object_set (G_OBJECT (video_sink), "async-handling", TRUE, NULL);



//    /* Set video Source */
//    g_object_set (G_OBJECT (rtspsrc), "location", argv[1], NULL);
//    g_object_set (G_OBJECT (rtspsrc), "do-rtcp", TRUE, NULL);
//    g_object_set (G_OBJECT (rtspsrc), "latency", 0, NULL);

//    //validate all
//    g_object_set (G_OBJECT (rtspsrc), "tls-validation-flags", G_TLS_CERTIFICATE_VALIDATE_ALL, NULL);
//    //g_object_set (G_OBJECT (rtspsrc), "tls-validation-flags", G_TLS_CERTIFICATE_INSECURE, NULL);


//    /* Putting a Message handler */
//    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
//    gst_bus_add_watch (bus, bus_call, loop);
//    gst_object_unref (bus);




//    //Make sure: Every elements was created ok
//    if (!pipeline || !rtspsrc || !rtph264depay || !h264parse || !avdec_h264 || !videoqueue0 || !videoconvert ) {
//        g_printerr ("One of the elements wasn't created... Exiting\n");
//        return -1;
//    }

//    // Add Elements to the Bin
//    gst_bin_add_many (GST_BIN (pipeline), rtspsrc, rtph264depay, h264parse, avdec_h264, videoqueue0, videoconvert, video_sink, NULL);

//    // Link confirmation
//    if (!gst_element_link_many (rtph264depay, h264parse, avdec_h264, NULL)){
//        g_warning ("Linking part (A)-1 Fail...");
//        return -2;
//    }
//    //Linking order is important -> start linking all elements from left (near rtspsrc) to right (near videosink)

//    // Link confirmation
//    if (!gst_element_link_many (videoqueue0, videoconvert, video_sink, NULL)){
//        g_warning ("Linking part (A)-2 Fail...");
//        return -4;
//    }


//    // Dynamic Pad Creation
//    if(! g_signal_connect (rtspsrc, "pad-added", G_CALLBACK (on_pad_added),rtph264depay))
//    {
//        g_warning ("Linking part (1) with part (A)-1 Fail...");
//    }

//    // Dynamic Pad Creation
//    if(! g_signal_connect (avdec_h264, "pad-added", G_CALLBACK (on_pad_added),videoqueue0))
//    {
//        g_warning ("Linking part (2) with part (A)-2 Fail...");
//    }


//    /* Run the pipeline */
//    g_print ("Playing: %s\n", argv[1]);
//    gst_element_set_state (pipeline, GST_STATE_PLAYING);
//    g_main_loop_run (loop);

//    /* Ending Playback */
//    g_print ("End of the Streaming... ending the playback\n");
//    gst_element_set_state (pipeline, GST_STATE_NULL);

//    /* Eliminating Pipeline */
//    g_print ("Eliminating Pipeline\n");
//    gst_object_unref (GST_OBJECT (pipeline));

//    return 0;
//}
