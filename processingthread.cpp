#include "processingthread.h"

ProcessingThread::ProcessingThread(ScaledView* glWidget): QThread()
{
    this->glWidget = glWidget;
}

void ProcessingThread::run()
{
    qDebug() << "Starting processing thread...";
    while(1)
    {
        if(!queue.isEmpty()) {
            GstSample *sample = queue.dequeue();
            GstBuffer *buffer = gst_sample_get_buffer(sample);
            GstMapInfo map;

            if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
                cv::Mat frame(cv::Size(1920, 1080), CV_8UC4, (char *)map.data, cv::Mat::AUTO_STEP); //4 kanal alpha ile RGBA
                this->glWidget->setData(frame);
                gst_buffer_unmap(buffer, &map);
            }
            gst_sample_unref(sample);
        }
    }
    qDebug() << "Stopping processing thread...";
}
