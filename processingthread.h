#ifndef PROCESSINGTHREAD_H
#define PROCESSINGTHREAD_H

// Qt
#include <QtCore/QThread>

#include <gst/gst.h>
#include "scaledview.h"
#include <opencv2/opencv.hpp>

class ProcessingThread : public QThread
{
    Q_OBJECT

    public:
        ProcessingThread(ScaledView *glWidget);
        QQueue<GstSample *> queue;
    protected:
        void run();
    private:
        ScaledView* glWidget;
};

#endif // PROCESSINGTHREAD_H
