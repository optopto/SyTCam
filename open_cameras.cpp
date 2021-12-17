#include <mainwindow.h>
#include "ui_mainwindow.h"
#include "globalVar.h"

void configure_defaults(CEmergentCamera* camera);
void CallBackFunc(int event, int x, int y, int flags, void* userdata);
int getH(CEmergentCamera* camera);
int getW(CEmergentCamera* camera);
int getmaxW(CEmergentCamera* camera);
int getmaxH(CEmergentCamera* camera);
int getMaxGain(CEmergentCamera* camera);
int getMinGain(CEmergentCamera* camera);
int getMaxExposure(CEmergentCamera* camera);
int getMinExposure(CEmergentCamera* camera);
int aproxGetRoiX(int roiX);
int aproxGetRoiY(int roiY);
int aproxRoiSlider(int roiSlider);

extern "C" void getFrameCam(uchar *Frame,uchar *Frame_2,uchar *Frame_3,uchar *Frame_4 ,int buffer);
extern "C" void testCuda_(CEmergentCamera *camera, CEmergentFrame *evtFrame);


void MainWindow::on_actionOpen_triggered()
{
    Mat mat;
    //    FILE *archivo;
    QMessageBox alertas;
    QTextStream stream_(stdout);
    struct GigEVisionDeviceInfo deviceInfo_[MAX_CAMERAS];
    unsigned int listcam_buf_size_ = MAX_CAMERAS;
    unsigned int count_;
    EVT_ListDevices(deviceInfo_, &listcam_buf_size_, &count_);
    CAMERAS = count_;
    int ReturnVal = SUCCESS;
    EVT_ERROR err = EVT_SUCCESS;
    arrLabel[0] = ui->label_6; arrLabel[1] = ui->label_7;
    arrLabel[2] = ui->label_8; arrLabel[3] = ui->label_9;
    arrRadioEnable[0] = ui->radioButton;
    arrRadioEnable[1] = ui->radioButton_2;
    arrRadioEnable[2] = ui->radioButton_3;
    arrRadioEnable[3] = ui->radioButton_4;
    arrRadioZoom[0] = ui->zoomButton;
    arrRadioZoom[1] = ui->zoomButton_2;
    arrRadioZoom[2] = ui->zoomButton_3;
    arrRadioZoom[3] = ui->zoomButton_4;
    arrButtonZoom[0] = ui->reset_zoom_1;
    arrButtonZoom[1] = ui->reset_zoom_2;
    arrButtonZoom[2] = ui->reset_zoom_3;
    arrButtonZoom[3] = ui->reset_zoom_4;
    done_close = false;
    for(int var = 0; var < 50; var++){widhtOffset[var] = var*16;}
    for(int var_ = 0; var_ < 154; var_++){heightOffset[var_] = var_*4;}

    for(int icam = MAX_CAMERAS-1; icam >= CAMERAS ; icam--){
        arrLabel[icam]->setText("No disponible");
        arrRadioEnable[icam]->setDisabled(true);
        arrRadioZoom[icam]->setDisabled(true);
        arrButtonZoom[icam]->setDisabled(true);
    }



    for (int cam = 0; cam < CAMERAS; cam++){
        camOpenT[cam] = QThread::create([=]{openCameras(cam,&camera[cam]);});
        camOpenT[cam]->start();
    }
    for(int cam = 0; cam < CAMERAS; cam ++){camOpenT[cam]->exit();}
    for(int cam = 0; cam < CAMERAS; cam ++){camOpenT[cam]->wait();}

    for (int cam = 0; cam < CAMERAS; cam++){
        EVT_CameraOpenStream(&camera[cam]);
        if (ReturnVal != SUCCESS)
        {   alertas.setText("CameraOpenStream: Error\n");
            alertas.exec();
            return; }
        evtFrame[cam].size_x = getW(&camera[cam]);
        evtFrame[cam].size_y = getH(&camera[cam]);
        evtFrame[cam].pixel_type = GVSP_PIX_BAYGB8;
        EVT_AllocateFrameBuffer(&camera[cam], &evtFrame[cam], EVT_FRAME_BUFFER_ZERO_COPY);
        EVT_CameraQueueFrame(&camera[cam], &evtFrame[cam]);
    }
    unsigned int frames_recd = 0;
    int ff = 0;
    ui->frame->setDisabled(false);

    while(true)
    {
        Mat cropedImage[4];
        Mat cI[4];
        char path_[200];
        int buffer = 0;
        buffer = 800*616;
        for(int cam = 0; cam < CAMERAS; cam++){
            EVT_CameraExecuteCommand(&camera[cam], "AcquisitionStart");
        }

        // archivo = fopen("/home/pc/Documentos/matlab_/imagen_.txt","a");

        for (int cam = 0; cam < CAMERAS; cam++)
        {   EVT_CameraQueueFrame(&camera[cam], &evtFrame[cam]);
            err = EVT_CameraGetFrame(&camera[cam], &evtFrame[cam], EVT_INFINITE);

            //testCuda_(&camera[0], &evtFrame[0]);



            if (err) {
                printf("EVT_CameraGetFrame Error!\n");
                break;
            }
            else{frames_recd++;}
            EVT_CameraExecuteCommand(&camera[cam], "AcquisitionStop");
        }

        getFrameCam(evtFrame[0].imagePtr,evtFrame[1].imagePtr,evtFrame[2].imagePtr,evtFrame[3].imagePtr, buffer);

        for (int cam = 0; cam < CAMERAS; cam++){
            sprintf(path_, "/media/pc/b889862b-2b8a-44b9-8595-8b3819d37d2b/imagenes/imagen_%d.bmp",cam+1);
            EVT_FrameSave(&evtFrame[cam], path_, EVT_FILETYPE_BMP, EVT_ALIGN_NONE);
            fullImage[cam] =  imread(path_);
            fullI[cam] = imread(path_);
            //  largoBuffer = 800*616;
        }



        //if(ff == 0){
        //mat = Mat(616, 800, CV_8UC1, evtFrame[0].imagePtr);
        //archivo = fopen("/home/pc/Documentos/matlab_/imagen_.txt","w");

        //for (int i = 0; i < largoBuffer ; i++ ) {
        //for (int j = 0 ; j < 800 ; j++){
        //cout << evtFrame[0].imagePtr[i] << endl;
        //  fprintf(archivo,"%u ",(int)evtFrame[0].imagePtr[i]);
        //}
        //  }

        /* uchar **array = new uchar *[mat.rows];
         for (int i=0; i<mat.rows; i++)
             array[i] = new uchar[mat.cols];

         for (int i = 0; i < mat.rows; i++)
             array[i] = mat.ptr<uchar>(i);


         for (int i = 0; i < mat.rows; i++)
         {
             for (int j = 0; j < mat.cols; j++)
             {
                 fprintf(archivo,"%d ",array[i][j]);
             }

         }*/
        //fclose(archivo);

        //}


        if(zoom_cam1){
            namedWindow("Cam 1 Zoom", 1);
            setMouseCallback("Cam 1 Zoom",CallBackFunc, NULL);
            if(x_-roi <= 0 || x_-roi <= 0 || yy_1+2*roi >= 616 || xx_1+2*roi >= 800){ int nothing = 0;
               }else{rectangle(fullImage[0], r , Scalar(0,0,255),3,8,0);}
            imshow("Cam 1 Zoom", fullImage[0]);
            waitKey(30);
        }else if(zoom_cam2){
            namedWindow("Cam 2 Zoom", 1);
            setMouseCallback("Cam 2 Zoom",CallBackFunc, NULL);
            rectangle(fullImage[1], r_2 , Scalar(0,255,0),3,8,0);
            imshow("Cam 2 Zoom", fullImage[1]);
            waitKey(30);
        }else if(zoom_cam3){
            namedWindow("Cam 3 Zoom", 1);
            setMouseCallback("Cam 3 Zoom",CallBackFunc, NULL);
            rectangle(fullImage[2], r_3 , Scalar(0,0,255),3,8,0);
            imshow("Cam 3 Zoom", fullImage[2]);
            waitKey(30);
        }else if(zoom_cam4){
            namedWindow("Cam 4 Zoom", 1);
            setMouseCallback("Cam 4 Zoom",CallBackFunc, NULL);
            rectangle(fullImage[3], r_4 , Scalar(255,255,0),3,8,0);
            imshow("Cam 4 Zoom", fullImage[3]);
            waitKey(30);
        }else{
            destroyAllWindows();
        }
        if(x_-roi <= 0 || x_-roi <= 0 || yy_1+2*roi >= 616 || xx_1+2*roi >= 800){
            int nn = 0;
        }else{ cropedImage[0] = fullI[0](r);}

        cropedImage[1] = fullI[1](r_2);
        cropedImage[2] = fullI[2](r_3);
        cropedImage[3] = fullI[3](r_4);
        if(zoom_cam1){
            if (cropedImage[0].empty()){cout << "No change size" << endl;}else
            {cI[0] = cropedImage[0];
                imwrite("/media/pc/b889862b-2b8a-44b9-8595-8b3819d37d2b/imagenes/imagen_1.bmp", cI[0]);}
        }else if(zoom_cam2){
            if (cropedImage[1].empty()){cout << " No change size" << endl;}else
            {   cI[1] = cropedImage[1];
                imwrite("/media/pc/b889862b-2b8a-44b9-8595-8b3819d37d2b/imagenes/imagen_2.bmp", cI[1]);}
        }else if (zoom_cam3){
            if (cropedImage[2].empty()){cout << " No change size" << endl;}else
            {   cI[2] = cropedImage[2];
                imwrite("/media/pc/b889862b-2b8a-44b9-8595-8b3819d37d2b/imagenes/imagen_3.bmp", cI[2]);}
        }else if(zoom_cam4){
            if (cropedImage[3].empty()){cout << " No change size" << endl;}else
            {   cI[3] = cropedImage[3];
                imwrite("/media/pc/b889862b-2b8a-44b9-8595-8b3819d37d2b/imagenes/imagen_4.bmp", cI[3]);}
        }
        for(int cam = 0; cam < CAMERAS;cam++){
            path.sprintf("/media/pc/b889862b-2b8a-44b9-8595-8b3819d37d2b/imagenes/imagen_%d.bmp",cam+1);
            //image = QImage(evtFrame[cam].imagePtr,getW(&camera[cam]), getH(&camera[cam]),QImage::Format_Grayscale8);
            image.load(path);
            image = image.scaled(arrLabel[cam]->size(),Qt::KeepAspectRatio);
            arrLabel[cam]->setAlignment(Qt::AlignHCenter);
            arrLabel[cam]->setPixmap(QPixmap::fromImage(image));
        }

        image.load("/home/pc/Documentos/matlab_/sumaImagenes.tiff");
        image = image.scaled(ui->label_suma->size(),Qt::KeepAspectRatio);
        ui->label_suma->setAlignment(Qt::AlignHCenter);
        ui->label_suma->setPixmap(QPixmap::fromImage(image));

        QTime dieTime = QTime::currentTime().addMSecs(1000/60);
        while( QTime::currentTime() < dieTime ){QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );}

        if(optRoi == true){
            xx_1 = aproxGetRoiX(xx_1);
            yy_1 = aproxGetRoiY(yy_1);
            r =  Rect(0,0,0,0);
            configure_user(&camera[0], gain, 30, exposure, 0, 2*roi, 2*roi,xx_1,yy_1);
            global_roi_x_1 = xx_1;
            global_roi_y_1 = yy_1;
            global_roi_h_1 = 2*roi;
            global_roi_w_1 = 2*roi;
            optRoi = false;
            zoom_cam1 = false;
            zc1 = false;
            ui->zoomButton->setChecked(false);
        }
        if(optRoi_2 == true){
            xx_2 = aproxGetRoiX(xx_2);
            yy_2 = aproxGetRoiY(yy_2);
            r_2 =  Rect(0,0,0,0);
            configure_user(&camera[1], gain, 30, exposure, 0, 2*roi_2, 2*roi_2,xx_2,yy_2);
            global_roi_x_2 = xx_2;
            global_roi_y_2 = yy_2;
            global_roi_h_2 = 2*roi_2;
            global_roi_w_2 = 2*roi_2;
            optRoi_2 = false;
            zoom_cam2 = false;
            zc2 = false;
            ui->zoomButton_2->setChecked(false);
        }
        if(optRoi_3 == true){
            xx_3 = aproxGetRoiX(xx_3);
            yy_3 = aproxGetRoiY(yy_3);
            r_3 =  Rect(0,0,0,0);
            configure_user(&camera[2], gain, 30, exposure, 0, 2*roi_3, 2*roi_3,xx_3,yy_3);
            global_roi_x_3 = xx_3;
            global_roi_y_3 = yy_3;
            global_roi_h_3 = 2*roi_3;
            global_roi_w_3 = 2*roi_3;
            optRoi_3 = false;
            zoom_cam3 = false;
            zc3 = false;
            ui->zoomButton_3->setChecked(false);
        }
        if(optRoi_4 == true){
            xx_4 = aproxGetRoiX(xx_4);
            yy_4 = aproxGetRoiY(yy_4);
            r_4 =  Rect(0,0,0,0);
            configure_user(&camera[3], gain, 30, exposure, 0, 2*roi_4, 2*roi_4,xx_4,yy_4);
            global_roi_x_4 = xx_4;
            global_roi_y_4 = yy_4;
            global_roi_h_4 = 2*roi_4;
            global_roi_w_4 = 2*roi_4;
            optRoi_4 = false;
            zoom_cam4 = false;
            zc4 = false;
            ui->zoomButton_4->setChecked(false);
        }

        ff = ff + 1;
        if(ff == 1){done = true; doneCap = true;}
        if(done == false){break;}
        if(doneCap == false){break;}
    }
    //done = false;
    if(done == false){
        for (int cam = 0; cam < CAMERAS; cam++){
            camCloseT[cam] = QThread::create([=]{closeCameras(cam,&camera[cam]);});
            camCloseT[cam]->start();
        }

        for(int cam = 0; cam < CAMERAS; cam ++){camOpenT[cam]->quit();}
        for(int cam = 0; cam < CAMERAS; cam ++){camOpenT[cam]->wait();}
        doneCap = true;
        //fclose(archivo);

    }
    else if (doneCap == false){
        /* for (int cam = 0; cam < CAMERAS; cam++){
            camCloseT[cam] = QThread::create([=]{closeCameras(cam,&camera[cam]);});
            camCloseT[cam]->start();
        }
        for(int cam = 0; cam < CAMERAS; cam ++){camOpenT[cam]->quit();}
        for(int cam = 0; cam < CAMERAS; cam ++){camOpenT[cam]->wait();}*/
        cout << "salio del ciclo sin cerrar" << endl;
    }
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata){
    if(event == EVENT_LBUTTONDOWN){
        cout << "ROI (" << x << ", " << y << ")" << endl;
        if(zc1 == true){
            if(x-roi <= 0 || y-roi <= 0 || y+2*roi >= 616 || x+2*roi >= 800){
            int n = 0;
            }else{
                x_ = x;
                y_ = y;
                xx_1 = x_-roi;
                yy_1 = y_-roi;
                r = Rect(xx_1,yy_1,2*roi,2*roi);
            }
        }else if(zc2 == true){
            if(x-roi_2 <= 0 || y-roi_2 <= 0 || y+2*roi_2 >= 616 || x+2*roi_2 >= 800){
                int n_2 = 0;
            }else{
                x_2 = x;
                y_2 = y;
                xx_2 = x_2-roi_2;
                yy_2 = y_2-roi_2;
                r_2 = Rect(xx_2,yy_2,2*roi_2,2*roi_2);
            }
        }
        else if(zc3 == true){
            if(x-roi_3 <= 0 || y-roi_3 <= 0 || y+2*roi_3 >= 616 || x+2*roi_3 >= 800){
                int n_3 = 0;
            }else{
                x_3 = x;
                y_3 = y;
                xx_3 = x_3-roi_3;
                yy_3 = y_3-roi_3;
                r_3 = Rect(xx_3,yy_3,2*roi_3,2*roi_3);
            }
        }
        else if (zc4 == true){
            if(x-roi_4 <= 0 || y-roi_4 <= 0 || y+2*roi_4 >= 616 || x+2*roi_4 >= 800){
                int n_4 = 0;
            }else{
                x_4 = x;
                y_4 = y;
                xx_4 = x_4-roi_4;
                yy_4 = y_4-roi_4;
                r_4 = Rect(xx_4,yy_4,2*roi_4,2*roi_4);
            }
        }
    }
    if(event == EVENT_LBUTTONDBLCLK){
        if(zc1 == true){optRoi = true;}else
            if(zc2 == true){optRoi_2 = true;}else
                if(zc3 == true){optRoi_3 = true;}else
                    if(zc4 == true){optRoi_4 = true;}
    }
}

void MainWindow::on_roiSlider_sliderMoved(int position) {
    QString roi_str = QString("%1").arg(position);
    ui->lineEdit_roi->setText(roi_str);
    if(zoom_cam1){
        roi = position/2;
        roi = aproxRoiSlider(roi);
        xx_1 = x_-roi;
        yy_1 = y_-roi;
    }
    else if(zoom_cam2){
        roi_2 = position;
        roi_2 = aproxRoiSlider(roi_2);
        xx_2 = x_2-roi_2;
        yy_2 = y_2-roi_2;
    }
    else if(zoom_cam3){
        roi_3 = position;
        roi_3 = aproxRoiSlider(roi_3);
        xx_3 = x_3-roi_3;
        yy_3 = y_3-roi_3;
    }
    else if(zoom_cam4){
        roi_4 = position;
        roi_4 = aproxRoiSlider(roi_4);
        xx_4 = x_4-roi_4;
        yy_4 = y_4-roi_4;
    }
    if(zoom_cam1){
        if(xx_1 - roi <= 0 || yy_1 - roi<= 0 || yy_1+2*roi >= 616 || xx_1+2*roi >= 800){int n = 0;}
        else{r = Rect(xx_1,yy_1,2*roi,2*roi);}
    }else if(zoom_cam2) {
        if(xx_2 - roi_2<= 0 || yy_2-roi_2 <= 0 || yy_2+2*roi_2 >= 616 || xx_2+2*roi_2 >= 808){int n = 0;}
        else{r_2 = Rect(xx_2,yy_2,2*roi_2,2*roi_2);}
    }else if (zoom_cam3){
        if(xx_3-roi_3 <= 0 || yy_3-roi_3 <= 0 || yy_3+2*roi_3 >= 616 || xx_3+2*roi_3 >= 808){ int n = 0;}
        else{r_3 = Rect(xx_3,yy_3,2*roi_3,2*roi_3);}
    }else if(zoom_cam4){
        if(xx_4-roi_4 <= 0 || yy_4-roi_4 <= 0 || yy_4+2*roi_4 >= 616 || xx_4+2*roi_4 >= 808){int n = 0;}
        else{r_4 = Rect(xx_4,yy_4,2*roi_4,2*roi_4);}
    }
}

void MainWindow::on_zoomButton_clicked(bool checked)
{
    zoom_cam1 = checked; zoom_cam2 = false;
    zoom_cam3 = false; zoom_cam4 = false;
    zc1 = zoom_cam1; zc2 = zoom_cam2;
    zc3 = zoom_cam3; zc4 = zoom_cam4;
}

void MainWindow::on_zoomButton_2_clicked(bool checked)
{
    zoom_cam1 = false; zoom_cam2 = checked;
    zoom_cam3 = false; zoom_cam4 = false;
    zc1 = zoom_cam1; zc2 = zoom_cam2;
    zc3 = zoom_cam3; zc4 = zoom_cam4;
}

void MainWindow::on_zoomButton_3_clicked(bool checked)
{
    zoom_cam1 = false; zoom_cam2 = false;
    zoom_cam3 = checked; zoom_cam4 = false;
    zc1 = zoom_cam1; zc2 = zoom_cam2;
    zc3 = zoom_cam3; zc4 = zoom_cam4;
}

void MainWindow::on_zoomButton_4_clicked(bool checked)
{
    zoom_cam1 = false; zoom_cam2 = false;
    zoom_cam3 = false; zoom_cam4 = checked;
    zc1 = zoom_cam1; zc2 = zoom_cam2;
    zc3 = zoom_cam3; zc4 = zoom_cam4;
}

void MainWindow::on_reset_zoom_1_clicked()
{
    if(done == true){
        if(camConfig_1 == true){
            global_roi_x_1 = 800;
            global_roi_y_1 = 616;
            global_roi_h_1 = 0;
            global_roi_w_1 = 0;
            configure_user(&camera[0], gain, 30, exposure, 0, 800, 616, 0, 0);
            configure_user(&camera[0], gain, 30, exposure, 0, 800, 616, 0, 0);
        }
        optRoi = false;
    }
}

void MainWindow::on_reset_zoom_2_clicked()
{
    if(done == true){
        if(camConfig_2 == true){
            global_roi_x_2 = 800;
            global_roi_y_2 = 616;
            global_roi_h_2 = 0;
            global_roi_w_2 = 0;
            configure_user(&camera[1], gain, 30, exposure, 1, 800, 616, 0, 0);
            configure_user(&camera[1], gain, 30, exposure, 1, 800, 616, 0, 0);
        }
        optRoi_2 = false;
    }
}

void MainWindow::on_reset_zoom_3_clicked()
{
    if(done == true){
        if(camConfig_3 == true){
            global_roi_x_3 = 800;
            global_roi_y_3 = 616;
            global_roi_h_3 = 0;
            global_roi_w_3 = 0;
            configure_user(&camera[2], gain, 30, exposure, 1, 800, 616, 0, 0);
            configure_user(&camera[2], gain, 30, exposure, 1, 800, 616, 0, 0);
        }
        optRoi_3 = false;
    }
}

void MainWindow::on_reset_zoom_4_clicked()
{
    if(done == true){
        if(camConfig_4 == true){
            global_roi_x_4 = 800;
            global_roi_y_4 = 616;
            global_roi_h_4 = 0;
            global_roi_w_4 = 0;
            configure_user(&camera[3], gain, 30, exposure, 1, 800, 616, 0, 0);
            configure_user(&camera[3], gain, 30, exposure, 1, 800, 616, 0, 0);
        }
        optRoi_4 = false;
    }
}

void MainWindow::on_lineEdit_roi_returnPressed(){
    int position = ui->lineEdit_roi->text().toInt();
    QString roi_str = QString("%1").arg(position);
    ui->lineEdit_roi->setText(roi_str);
    ui->roiSlider->setValue(position/2);
    if(zoom_cam1){
        roi = position/2;
        xx_1 = x_-roi;
        yy_1 = y_-roi;
        global_roi_x_1 = xx_1;
        global_roi_y_1 = yy_1;
        global_roi_h_1 = 2*roi;
        global_roi_w_1 = 2*roi;

    }
    else if(zoom_cam2){
        roi_2 = position/2;
        xx_2 = x_2-roi_2;
        yy_2 = y_2-roi_2;
        global_roi_x_1 = xx_2;
        global_roi_y_1 = yy_2;
        global_roi_h_1 = 2*roi_2;
        global_roi_w_1 = 2*roi_2;}
    else if(zoom_cam3){
        roi_3 = position/2;
        xx_3 = x_-roi_3;
        yy_3 = y_-roi_3;
        global_roi_x_1 = xx_3;
        global_roi_y_1 = yy_3;
        global_roi_h_1 = 2*roi_3;
        global_roi_w_1 = 2*roi_3;
    }
    else if(zoom_cam4){
        roi_4 = position/2;
        xx_3 = x_-roi_4;
        yy_3 = y_-roi_4;
        global_roi_x_1 = xx_4;
        global_roi_y_1 = yy_4;
        global_roi_h_1 = 2*roi_4;
        global_roi_w_1 = 2*roi_4;
    }
    if(zoom_cam1){
        if(xx_1 -roi <= 0 || yy_1 -roi<= 0 || yy_1+2*roi >= 616 || xx_1+2*roi >= 808){
            int n = 0;
        }else{
            r = Rect(xx_1,yy_1,2*roi,2*roi);
        }
    }else if(zoom_cam2) {
        if(xx_2 - roi_2<= 0 || yy_2-roi_2 <= 0 || yy_2+2*roi_2 >= 616 || xx_2+2*roi_2 >= 808){
            int n = 0;
        }else{
            r_2 = Rect(xx_2,yy_2,2*roi_2,2*roi_2);
        }
    }else if (zoom_cam3){
        if(xx_3-roi_3 <= 0 || yy_3-roi_3 <= 0 || yy_3+2*roi_3 >= 616 || xx_3+2*roi_3 >= 808){
            int n = 0;
        }else{
            r_3 = Rect(xx_3,yy_3,2*roi_3,2*roi_3);
        }
    }else if(zoom_cam4){
        if(xx_4-roi_4 <= 0 || yy_4-roi_4 <= 0 || yy_4+2*roi_4 >= 616 || xx_4+2*roi_4 >= 808){
            int n = 0;
        }else{
            r_4 = Rect(xx_3,yy_4,2*roi_4,2*roi_4);
        }
    }
}

int aproxGetRoiX(int roiX){
    int x_4_[2];
    int comparar = 0;
    for(int var = 0; var < 50; var++){
        comparar = widhtOffset[var]-roiX;
        if(comparar > 0){
            x_4_[0] = qFabs(widhtOffset[var-1]-roiX);
            x_4_[1] = qFabs(widhtOffset[var]-roiX);
            if(x_4_[0] > x_4_[1]){roiX = widhtOffset[var];break;
            }else{roiX = widhtOffset[var-1];break;}
        }else if(comparar == 0){roiX = widhtOffset[var];break;}
    }
    return roiX;
}
int aproxGetRoiY(int roiY){
    int y_4_[2];
    int comparar = 0;
    for(int var_ = 0; var_ < 154; var_++){
        comparar = heightOffset[var_]-roiY;
        if(comparar > 0){
            y_4_[0] = qFabs(heightOffset[var_-1]-roiY);
            y_4_[1] = qFabs(heightOffset[var_]-roiY);
            if(y_4_[0] > y_4_[1]){roiY = heightOffset[var_];break;
            }else{roiY = heightOffset[var_-1];break;}
        }else if(comparar == 0){roiY = heightOffset[var_];break;}
    }
    return roiY;
}

int aproxRoiSlider(int roiSlider){
    int x_4_[2];
    int comparar = 0;
    for(int var = 0; var < 50; var++){
        comparar = widhtOffset[var]-roiSlider;
        if(comparar > 0){
            x_4_[0] = qFabs(widhtOffset[var-1]-roiSlider);
            x_4_[1] = qFabs(widhtOffset[var]-roiSlider);
            if(x_4_[0] > x_4_[1]){roiSlider = widhtOffset[var];break;
            }else{roiSlider = widhtOffset[var-1];break;}
        }else if(comparar == 0){roiSlider = widhtOffset[var];break;}
    }
    return roiSlider;
}
