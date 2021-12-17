#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <globalVar.h>

void configure_defaults(CEmergentCamera* camera);
int getH(CEmergentCamera* camera);
int getW(CEmergentCamera* camera);
int getmaxW(CEmergentCamera* camera);
int getmaxH(CEmergentCamera* camera);
int getMaxGain(CEmergentCamera* camera);
int getMinGain(CEmergentCamera* camera);
int getMaxExposure(CEmergentCamera* camera);
int getMinExposure(CEmergentCamera* camera);


int MainWindow::openCameras(int cam, CEmergentCamera *cam_p){
    int ReturnVal = SUCCESS;
    QString openCam;
    QMessageBox alertas;
    unsigned int listcam_buf_size = MAX_CAMERAS;
    unsigned int count;
    EVT_ListDevices(deviceInfo, &listcam_buf_size, &count);
    ReturnVal = EVT_CameraOpen(cam_p, &deviceInfo[cam]);
    configure_defaults(cam_p);
    configure_user(cam_p, gain, 30, exposure, cam, global_roi_w_1, global_roi_h_1,offset_x,offset_y);
    if(cam == 0){
        global_gain_max = getMaxGain(cam_p);
        global_gain_min = getMinGain(cam_p);
        global_expo_max = getMaxExposure(cam_p);
        global_expo_min = getMinExposure(cam_p);
        ui->gainSlider->setMaximum(global_gain_max);
        ui->gainSlider->setMinimum(global_gain_min);
        ui->exposureSlider->setMaximum(global_expo_max);
        ui->exposureSlider->setMinimum(global_expo_min);
        if (ReturnVal != SUCCESS)
        {
            openCam.sprintf("Error: No es posible abrir la camara %d", cam);
            alertas.setText(openCam);
            alertas.exec();
            exit(-1);
        }
    }
    return 0;
}

int MainWindow::closeCameras(int cam, CEmergentCamera *cam_p){
    done_close = true;
    EVT_CameraExecuteCommand(cam_p, "AcquisitionStop");
    EVT_CameraCloseStream(cam_p);
    configure_defaults(cam_p);
    frame_rate = ui->frame_Edit->text().toInt();
    configure_user(cam_p, gain, frame_rate, exposure,cam,global_roi_w_1, global_roi_h_1,global_roi_x_1,global_roi_y_1);
    EVT_CameraClose(cam_p);
    return 0;
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(done_close == false){
        QMessageBox close_error;
        close_error.setText("Error. Remember close the cameras");
        close_error.exec();
        event->ignore();
    }else if(done_close == true){
      /*  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Alert!",
                                                                   tr("Are you sure?\n"),
                                                                   QMessageBox::No | QMessageBox::Yes,
                                                                   QMessageBox::No);
        if (resBtn != QMessageBox::Yes){event->ignore();}*/
        event->accept();
    }
}
