#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>
#include <mutex>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QElapsedTimer>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //glWidget = new ScaledView(ui->centralwidget);

    th1 = std::async([&]{
        if(cam1.open(STREAM1,cv::CAP_GSTREAMER)){
            qDebug("Cam1 is successfully opened!\n");

            eventForCameraChoose(Cameras::Cam1);
            return true;
        }else{
            qDebug("Cam1 is failed open!\n");
        }
        return false;
    });

    /*th2 = std::async([&]{
        if(cam2.open(STREAM2,cv::CAP_GSTREAMER)){
            std::cout << "Cam2 is successfully opened!\n";
            return true;
        }else{
            std::cout << "Cam2 is failed open!\n";
        }
        return false;
    });

    th3 = std::async([&]{
        if(cam3.open(STREAM3,cv::CAP_ANY)){
            std::cout << "Cam3 is successfully opened!\n";
            return true;
        }else{
            std::cout << "Cam3 is failed open!\n";
        }
        return false;
    });

    th4 = std::async([&]{
        if(cam4.open(STREAM4,cv::CAP_GSTREAMER)){
            std::cout << "Cam4 is successfully opened!\n";
            return true;
        }else{
            std::cout << "Cam4 is failed open!\n";
        }
        return false;
    });*/

    ui->videoLabel->setVisible(true);
    ui->smallVideoLabel1->setVisible(false);
    ui->smallVideoLabel2->setVisible(false);
    ui->smallVideoLabel3->setVisible(false);
    ui->smallVideoLabel4->setVisible(false);

    if(th1.wait_for(std::chrono::milliseconds(500)) == std::future_status::ready){
        th1.get();
        qDebug("Cam1 ok!\n");

    }

    /*if(th2.wait_for(std::chrono::milliseconds(500)) == std::future_status::ready){
        th2.get();
    }

    if(th3.wait_for(std::chrono::milliseconds(500)) == std::future_status::ready){
        th3.get();
    }

    if(th4.wait_for(std::chrono::milliseconds(500)) == std::future_status::ready){
        th4.get();
    }*/

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

void MainWindow::videoCaptureThread()
{
    cv::VideoCapture *cap=nullptr;
    while(true){
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

        if(currentCam!=Cameras::AllCam){
            loopForDiscreteVideo(cap);
        }else{
            loopForSmall4Videos();
        }
    }
}

void MainWindow::eventForCameraChoose(Cameras camera){
    qDebug("Camera chosen\n");
    flag=true;
    currentCam = camera;
}

void MainWindow::setVideoLabel(VideoLabel video){
    if(video==VideoLabel::DiscreteVideo){
        ui->videoLabel->setVisible(true);
        ui->smallVideoLabel1->setVisible(false);
        ui->smallVideoLabel2->setVisible(false);
        ui->smallVideoLabel3->setVisible(false);
        ui->smallVideoLabel4->setVisible(false);
    }
    else if(video==VideoLabel::Small4Videos){
        ui->videoLabel->setVisible(false);
        ui->smallVideoLabel1->setVisible(true);
        ui->smallVideoLabel2->setVisible(true);
        ui->smallVideoLabel3->setVisible(true);
        ui->smallVideoLabel4->setVisible(true);
    }
}

cv::Mat MainWindow::make2xZoom(const cv::Mat&mat){
    cv::Mat _mat(mat.rows/2,mat.cols/2,mat.type());
    int _col=0;
    for(int i=mat.cols/2-_mat.cols/2;i<mat.cols/2+_mat.cols/2;++i,++_col){
        int _row=0;
        for(int j=mat.rows/2-_mat.rows/2;j<mat.rows/2+_mat.rows/2;++j,++_row){
            _mat.at<cv::Vec3b>(_row,_col)=mat.at<cv::Vec3b>(j,i);
        }
    }
    return _mat;
}

void MainWindow::loopForDiscreteVideo(cv::VideoCapture *cap)
{
    qDebug("loopForDiscreteVideo");
    setVideoLabel(VideoLabel::DiscreteVideo);
    QElapsedTimer *t = new QElapsedTimer();
    int fps = 0;
    t->start();
    while (true) {
        static cv::Mat frame;
        static cv::Mat frame2xZoom;
        if(cap->read(frame) && !frame.empty()){

            if(flag2xZoom){
                frame2xZoom = make2xZoom(frame);
                ui->videoLabel->setPixmap(QPixmap::fromImage(QImage(frame2xZoom.data, frame2xZoom.cols, frame2xZoom.rows, frame2xZoom.step, QImage::Format_BGR888).scaled(ui->videoLabel->width(),ui->videoLabel->height())));
            }else{
                cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
                ui->glWidget->setQImage(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888));
                //qDebug("1\n");
                //ui->glWidget->update();
                //ui->videoLabel->setPixmap(QPixmap::fromImage(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888).scaled(ui->videoLabel->width(),ui->videoLabel->height())));
            }
        }
        else
        {
            qDebug("empty\n");
            //ui->videoLabel->setPixmap(QPixmap::fromImage(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888).scaled(ui->videoLabel->width(),ui->videoLabel->height())));
        }

        if(flag){
            qDebug("exit\n");
            break;
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        fps++;
        if(t->hasExpired(1000)) {
            qDebug("FPS: %d\n", fps);
            fps = 0;
            t->restart();
        }
    }
}

void MainWindow::loopForSmall4Videos()
{
    setVideoLabel(VideoLabel::Small4Videos);
    while (true) {

        static cv::Mat frame1;
        static cv::Mat frame2;
        static cv::Mat frame3;
        static cv::Mat frame4;


        if(cam1.read(frame1))
            ui->smallVideoLabel1->setPixmap(QPixmap::fromImage(QImage(frame1.data, frame1.cols, frame1.rows, frame1.step, QImage::Format_BGR888).scaled(ui->smallVideoLabel1->width(),ui->smallVideoLabel1->height())));

        if(cam2.read(frame2))
            ui->smallVideoLabel2->setPixmap(QPixmap::fromImage(QImage(frame2.data, frame2.cols, frame2.rows, frame2.step, QImage::Format_BGR888).scaled(ui->smallVideoLabel2->width(),ui->smallVideoLabel2->height())));

        if(cam3.read(frame3))
            ui->smallVideoLabel3->setPixmap(QPixmap::fromImage(QImage(frame3.data, frame3.cols, frame3.rows, frame3.step, QImage::Format_BGR888).scaled(ui->smallVideoLabel3->width(),ui->smallVideoLabel3->height())));

        if(cam4.read(frame4))
            ui->smallVideoLabel4->setPixmap(QPixmap::fromImage(QImage(frame4.data, frame4.cols, frame4.rows, frame4.step, QImage::Format_BGR888).scaled(ui->smallVideoLabel4->width(),ui->smallVideoLabel4->height())));

        if(flag){
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void MainWindow::on_btnCam1_clicked()
{
    eventForCameraChoose(Cameras::Cam1);
}

void MainWindow::on_btnCam2_clicked()
{
    eventForCameraChoose(Cameras::Cam2);
}

void MainWindow::on_btnCam3_clicked()
{
    eventForCameraChoose(Cameras::Cam3);
}

void MainWindow::on_btnCam4_clicked()
{
    eventForCameraChoose(Cameras::Cam4);
}

void MainWindow::on_btnAllCam_clicked()
{
    eventForCameraChoose(Cameras::AllCam);
}

void MainWindow::on_btnZoom2x_clicked()
{
    flag2xZoom=!flag2xZoom;
}
