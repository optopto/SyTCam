#include "mainwindow.h"
#include "ui_mainwindow.h"

void configure_defaults(CEmergentCamera* camera);
int getH(CEmergentCamera* camera);
int getW(CEmergentCamera* camera);
QString dir_save_files;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int j = 0;
    lens_port_one = "";
    lens_port_two = "";
    lens_port_three = "";
    lens_port_four ="";
    for (int i = 0; i < 4 ; i++ ) {lens_avail[i] = false;}
    lente_one = new QSerialPort;
    lente_two = new QSerialPort;
    lente_three = new QSerialPort;
    lente_four = new QSerialPort;
    qDebug() << "Puertos disponibles: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Vendor: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "ID: " << serialPortInfo.vendorIdentifier();
            lens_avail[j] = true;
            if(j == 0){
                lens_port_one=serialPortInfo.portName();
            }
            if(j == 1){
                lens_port_two=serialPortInfo.portName();
            }
        }
        qDebug() << "product : " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "product: " << serialPortInfo.productIdentifier();
        }
        qDebug() << "Port name: " << serialPortInfo.portName();
        j++;
    }
    if(lens_avail[0]){
        lente_one->setPortName(lens_port_one);
        lente_one->open(QSerialPort::WriteOnly);
        lente_one->setBaudRate(QSerialPort::Baud38400);
        lente_one->setDataBits(QSerialPort::Data8);
        lente_one->setParity(QSerialPort::NoParity);
        lente_one->setStopBits(QSerialPort::OneStop);
        lente_one->setFlowControl(QSerialPort::NoFlowControl);
    }
    if(lens_avail[1]){
        lente_two->setPortName(lens_port_two);
        lente_two->open(QSerialPort::WriteOnly);
        lente_two->setBaudRate(QSerialPort::Baud38400);
        lente_two->setDataBits(QSerialPort::Data8);
        lente_two->setParity(QSerialPort::NoParity);
        lente_two->setStopBits(QSerialPort::OneStop);
        lente_two->setFlowControl(QSerialPort::NoFlowControl);
    }

    ui->label_dir_save->setText(save_directory[0]);
    ui->progressBar->setVisible(false);
    ui->frame->setDisabled(true);
}

MainWindow::~MainWindow()
{
    destroyAllWindows();
    delete ui;
}


int MainWindow::worker_thread(CEmergentCamera *cam_p,int frame_to_recv,int cameraWork)
{   unsigned int we1 = 0;
    unsigned int he1 = 0;
    we1 = getW(cam_p);
    he1 = getH(cam_p);
    CEmergentAVIFile videoBuffer;
    videoBuffer.fps=60;
    videoBuffer.codec = EVT_CODEC_NONE;
    videoBuffer.width = we1;
    videoBuffer.height = he1;
    EVT_ERROR err = EVT_SUCCESS;
    bool buffer_used = false, buffer_recd = false;
    CEmergentFrame p_frms[FIXEDBUFFER];
    CEmergentFrame evtFrameRecv[FIXEDBUFFER];
    int frame_count = 0;
    int dropped_frames = 0;
    unsigned short id_prev= 0;
    bool  done_ = false;
    int save_frame = 0;
    QString file_number;
    QString file_number_;
    QImage image_preview;
    file_number = QString(dir_save_files + "%1/").arg(cameraWork+1);
    file_number_ = QString(file_number + "my_video_%1.avi").arg(cameraWork+1);
    string file_name = file_number_.toStdString();
    strcpy(videoBuffer.fileName,file_name.c_str());
    EVT_AVIOpen(&videoBuffer);
    int frames = 0;
    int rep = 0;
    for (frames = 0; frames < FIXEDBUFFER; frames++)
    {
        p_frms[frames].size_x = we1;
        p_frms[frames].size_y = he1;
        p_frms[frames].pixel_type = GVSP_PIX_MONO8;
        err = EVT_AllocateFrameBuffer(cam_p, &p_frms[frames], EVT_FRAME_BUFFER_ZERO_COPY);
        if (err){cout<<"Error"<<endl;}
        err = EVT_CameraQueueFrame(cam_p, &p_frms[frames]);
        if (err){cout<<"Error EVT: Camera Queue Frame error uno "<<endl;}
    }
    ui->progressBar->setValue(25);
    while ((d1 && d2 && d3 && d4) == false) {
        if (cameraWork == 0){d1 = true;}
        if (cameraWork == 1 || CAMERAS < 2) {d2 = true;}
        if (cameraWork == 2 || CAMERAS < 3) {d3 = true;}
        if (cameraWork == 3 || CAMERAS < 4) {d4 = true;}
    }
    sleep(1);
    err = EVT_CameraExecuteCommand(cam_p, "AcquisitionStart");
    if (err)
    {
        cout<<"Error EVT: Acquisition Start error dos"<<endl;
    }
    ui->progressBar->setValue(50);
    while(!done_){
        if (buffer_used){err = EVT_CameraQueueFrame(cam_p, &evtFrameRecv[save_frame-1]);
            if (err){cout<<"Error EVT: Camera Queue Frame error"<<endl;}
            buffer_used = false;}
        err = EVT_CameraGetFrame(cam_p, &evtFrameRecv[save_frame], EVT_INFINITE);
        if (!err){
            if (((evtFrameRecv[save_frame].frame_id) != id_prev + 1) && (frame_count != 0)){
                dropped_frames++;
                buffer_recd = false;
            }
            else{frame_count++; buffer_recd = true;}
        }
        else{dropped_frames++; buffer_recd = false;}
        if (evtFrameRecv[save_frame].frame_id == 65535){id_prev = 0;}
        else{id_prev = evtFrameRecv[save_frame].frame_id;}
        buffer_used = true;
        if (frame_count >= frame_to_recv){done_ = true;}
        if (!buffer_recd) continue;
        if (save_frame == 200-1){
            for (save_frame = 0; save_frame < 200; save_frame++) {
                // char filename[100];
                /* file_number_ = QString(file_number + "%1.tiff").arg((200*rep)+save_frame+1);
                string file_name = file_number_.toStdString();
                strcpy(filename,file_name.c_str());
                EVT_FrameSave(&evtFrameRecv[save_frame], filename, EVT_FILETYPE_TIF, EVT_ALIGN_NONE);*/
                EVT_AVIAppend(&videoBuffer,&evtFrameRecv[save_frame]);
                }
            rep++;
            save_frame = -1;
            buffer_used = false;        }

        save_frame++;
    }
    EVT_AVIClose(&videoBuffer);
    ui->progressBar->setValue(70);
    for (frames = 0; frames < FIXEDBUFFER; frames++){EVT_ReleaseFrameBuffer(cam_p, &p_frms[frames]);}
    system("sync && echo 3 > /proc/sys/vm/drop_caches");

    ui->progressBar->setValue(75);
    while ((d1 || d2 || d3 || d4) == true) {
        if (cameraWork == 0){d1 = false;}
        if (cameraWork == 1 || CAMERAS < 2) {d2 = false;}
        if (cameraWork == 2 || CAMERAS < 3) {d3 = false;}
        if (cameraWork == 3 || CAMERAS < 4) {d4 = false;}
    }



    ui->progressBar->setValue(100);

    closeCameras(cameraWork,cam_p);


    return 0;
}



void MainWindow::on_actionCapture_Data_triggered()
{
    ui->frame->setDisabled(true);
    frame_rate = ui->frame_Edit->text().toInt();
    time_capture =ui->time_Edit->text().toInt();
    ui->progressBar->setVisible(true);
    QString timeFrame = QString("Frame Rate: %1 [Frames] \nTime capture: %2 [s]").arg(frame_rate).arg(time_capture);
    ui->label_thread_0->setText(timeFrame);
    int frame_to_recv = time_capture*frame_rate;
    CEmergentFrame evtFrame[BUFFCAPTURE];
    createQtdir(frame_rate,CAMERAS);
    dir_save_files = exist;
    ui->progressBar->setValue(10);
    doneCap = false;

    for(int cam=0 ;cam < CAMERAS; cam++){
        EVT_CameraSetUInt32Param(&camera[cam], "FrameRate", frame_rate);
    }

    for (int cam = 0; cam < CAMERAS; cam++){
        camThreadOne[cam] = QThread::create([=]{worker_thread(&camera[cam],frame_to_recv,cam);});
        camThreadOne[cam]->start();
    }
    for(int cam = 0; cam < CAMERAS; cam ++){camThreadOne[cam]->quit();}
    for(int cam = 0; cam < CAMERAS; cam ++){camThreadOne[cam]->wait(0);}





    return;
}




