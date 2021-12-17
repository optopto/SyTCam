#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "info.h"
//#include "ui_info.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTextStream>
#include <QMainWindow>
#include <QImage>
#include <QString>
#include <QTime>
#include <QLineEdit>
#include <QCloseEvent>

#include <thread>

#include <EmergentCameraAPIs.h>
#include <emergentframe.h>
#include <EvtParamAttribute.h>
#include <gigevisiondeviceinfo.h>
#include <EmergentFrameSave.h>
#include <EmergentAVIFile.h>

#ifdef _MSC_VER
  #include <windows.h>
#else
  #include <pthread.h>
  #include <sys/time.h>
  #include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "string"
#include <errno.h>
#include <sys/stat.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <fstream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/viz/types.hpp>
#include <iostream>

#include <QFile>
#include <QFileDialog>

#include <QThread>
#include <QtMath>
#include <QtCore>

#include <cstdio>
#include <stdio.h>

#include <libssh/libssh.h>

#define BUFFCAPTURE 200
#define MAX_CAMERAS 4
#define BUFFPREVIEW 10
#define SUCCESS 0
#define FIXEDBUFFER 500
using namespace std;
using namespace Emergent;
using namespace cv;

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    friend class setting;
private slots:
    void configure_user(CEmergentCamera* camera, int GAIN_VAL, int frame_rate, int exposition, int info_static, int we, int he, int offset_x, int offset_y);
    void on_gainSlider_sliderMoved(int position);
    void on_exposureSlider_sliderMoved(int position);
    void on_radioButton_clicked(bool checked);
    void on_radioButton_2_clicked(bool checked);
    void on_radioButton_3_clicked(bool checked);
    void on_radioButton_4_clicked(bool checked);
    void on_aperturaButton_clicked();
    void on_maperturaButton_clicked();
    void on_resetFocButton_clicked();
    void on_focusMButton_3_clicked();
    void on_focusButton_clicked();
    void on_resetApButton_clicked();
    void on_zoomButton_clicked(bool checked);
    void on_roiSlider_sliderMoved(int position);
    void on_zoomButton_2_clicked(bool checked);
    void on_zoomButton_3_clicked(bool checked);
    void on_zoomButton_4_clicked(bool checked);
    void on_actionSave_Directory_triggered();
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void createQtdir(int fr, int camM);
    void on_actionCapture_Data_triggered();
    void on_reset_zoom_1_clicked();
    void on_lineEdit_gain_returnPressed();
    void on_lineEdit_exposure_returnPressed();
    void on_lineEdit_roi_returnPressed();
    void on_reset_zoom_2_clicked();
    int worker_thread(CEmergentCamera *cam_p, int frame_to_recv, int cameraWork);
    int openCameras(int hilo, CEmergentCamera *camera);
    int closeCameras(int hilo, CEmergentCamera *camera);
    void on_reset_zoom_3_clicked();
    void on_reset_zoom_4_clicked();
    void configureCamFunc();
    void configureLensFunc(char parameter);
    void closeEvent(QCloseEvent *event);


private:
    Ui::MainWindow *ui;
    QImage image, image_two;
    QImage image_three, image_for;
    QString name_file;
    QString path, path_two;
    QString path_three, path_for;
    CEmergentFrame evtFrame[BUFFPREVIEW];
    struct GigEVisionDeviceInfo deviceInfo[MAX_CAMERAS];
    CEmergentCamera camera[MAX_CAMERAS];
    int CAMERAS = 0;
    bool done = false;
    bool doneCap = false;
    int gain = 600;
    int exposure = 600;
    int slider_width = 800;
    int slider_heigth = 616;
    int offset_x = 0;
    int offset_y = 0;
    bool camConfig_1 = false;
    bool camConfig_2 = false;
    bool camConfig_3 = false;
    bool camConfig_4 = false;
    bool zoom_cam1 = false;
    bool zoom_cam2 = false;
    bool zoom_cam3 = false;
    bool zoom_cam4 = false;
    unsigned int iris = 0;
    unsigned int focus = 5000;
    QSerialPort *lente_one;
    QSerialPort *lente_two;
    QSerialPort *lente_three;
    QSerialPort *lente_four;
    QString lens_port_one;
    QString lens_port_two;
    QString lens_port_three;
    QString lens_port_four;
    bool lens_avail[4];
    quint64 readVa;
    Mat fullImage[4];
    Mat fullI[4];
    QMessageBox close_alert;
    QStringList save_directory = QStringList("/home/pc/");
    QString exist;
    int roi_capture_ = 0;
    int global_roi_x_1 = 0;
    int global_roi_y_1 = 0;
    int global_roi_h_1 = 616;
    int global_roi_w_1 = 800;
    int roi_capture_2 = 0;
    int global_roi_x_2 = 0;
    int global_roi_y_2 = 0;
    int global_roi_h_2 = 616;
    int global_roi_w_2 = 800;
    int roi_capture_3 = 0;
    int global_roi_x_3 = 0;
    int global_roi_y_3 = 0;
    int global_roi_h_3 = 616;
    int global_roi_w_3 = 800;
    int roi_capture_4 = 0;
    int global_roi_x_4 = 0;
    int global_roi_y_4 = 0;
    int global_roi_h_4 = 616;
    int global_roi_w_4 = 800;
    int global_gain_max = 0;
    int global_gain_min = 0;
    int global_expo_max = 0;
    int global_expo_min = 0;
    QMutex mutex;
    QMutex mutex_cam;
    QMutex mutexCloseCam;
    QThread *camThreadOne[4];
    QThread *camOpenT[4];
    QThread *camCloseT[4];
    QThread *infoLabel;
    QLabel *arrLabel[4];
    QRadioButton *arrRadioEnable[4];
    QRadioButton *arrRadioZoom[4];
    QPushButton *arrButtonZoom[4];
    int frame_rate = 0;
    int time_capture = 0;
    bool d1 = false;
    bool d2 = false;
    bool d3 = false;
    bool d4 = false;
    bool done_close = true;

signals:
};

#endif // MAINWINDOW_H
