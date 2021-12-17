#include "mainwindow.h"
#include "ui_mainwindow.h"

char* next_token;
void configure_defaults(CEmergentCamera* camera)
{
    unsigned int param_val_max;
    unsigned int width_max, height_max;
    const unsigned long enumBufferSize = 1000;
    unsigned long enumBufferSizeReturn = 0;
    char enumBuffer[enumBufferSize];
    EVT_CameraGetEnumParamRange(camera, "PixelFormat", enumBuffer, enumBufferSize, &enumBufferSizeReturn);
    char* enumMember = strtok_s(enumBuffer, ",", &next_token);
    EVT_CameraSetEnumParam(camera, "PixelFormat", enumMember);
    EVT_CameraSetUInt32Param(camera, "FrameRate", 30);
    EVT_CameraSetUInt32Param(camera, "OffsetX", 0);
    EVT_CameraSetUInt32Param(camera, "OffsetY", 0);
    EVT_CameraGetUInt32ParamMax(camera, "Width", &width_max);
    EVT_CameraSetUInt32Param(camera, "Width", width_max);
    EVT_CameraGetUInt32ParamMax(camera, "Height", &height_max);
    EVT_CameraSetUInt32Param(camera, "Height", height_max);
    EVT_CameraSetEnumParam(camera, "AcquisitionMode", "MultiFrame");
    EVT_CameraSetUInt32Param(camera, "AcquisitionFrameCount", 200);
    EVT_CameraSetEnumParam(camera, "TriggerSelector", "AcquisitionStart");
    EVT_CameraSetEnumParam(camera, "TriggerMode", "Off");
    EVT_CameraSetEnumParam(camera, "TriggerSource", "Software");
    EVT_CameraSetEnumParam(camera, "BufferMode", "Off");
    EVT_CameraSetUInt32Param(camera, "BufferNum", 0);
    EVT_CameraGetUInt32ParamMax(camera, "GevSCPSPacketSize", &param_val_max);
    EVT_CameraSetUInt32Param(camera, "GevSCPSPacketSize", param_val_max);
    EVT_CameraSetUInt32Param(camera, "Gain", 256);
    EVT_CameraSetUInt32Param(camera, "Offset", 0);
    EVT_CameraSetBoolParam(camera, "LUTEnable", false);
    EVT_CameraSetBoolParam(camera, "AutoGain",false);
}

void MainWindow::configure_user(CEmergentCamera* camera, int GAIN_VAL, int frame_rate, int exposition, int info_static, int we, int he,int offset_x, int offset_y)
{
    const unsigned long StringSize = 256;
    unsigned long StringSizeReturn = 0;
    char StringBuffer[StringSize];
    char ip[16];
    char friendName[128];
    QString info("Status Camera: ");
    QString info_particular;
    unsigned int frame_rate_max, frame_rate_status;
    unsigned int width_max, height_max;
    int temp;
    info_particular.sprintf("%d: \n", info_static + 1);
    info.append(info_particular);
    EVT_CameraGetInt32Param(camera, "SensTemp", &temp);
    info_particular.sprintf("Temperature: %d C \n", temp);
    info.append(info_particular);
    EVT_CameraGetStringParam(camera, "DeviceVendorName", StringBuffer, StringSize, &StringSizeReturn);
    info_particular.sprintf("Device Name: %s\n", StringBuffer);
    info.append(info_particular);
    EVT_CameraGetStringParam(camera, "DeviceModelName", StringBuffer, StringSize, &StringSizeReturn);
    info_particular.sprintf("Device Model: %s\n", StringBuffer);
    info.append(info_particular);
    EVT_CameraGetStringParam(camera, "DeviceVersion", StringBuffer, StringSize, &StringSizeReturn);
    info_particular.sprintf("Version: %s\n", StringBuffer);
    info.append(info_particular);
    EVT_CameraGetStringParam(camera, "DeviceSerialNumber", StringBuffer, StringSize, &StringSizeReturn);
    info_particular.sprintf("Serial Number: %s\n", StringBuffer);
    info.append(info_particular);
    EVT_CameraGetStringParam(camera, "DeviceFirmwareVersion", StringBuffer, StringSize, &StringSizeReturn);
    info_particular.sprintf("Firmware Version: %s\n", StringBuffer);
    info.append(info_particular);
    EVT_CameraGetUInt32ParamMax(camera, "Height", &height_max);
    EVT_CameraGetUInt32ParamMax(camera, "Width", &width_max);
    info_particular.sprintf("Max Res: %d x %d \n", height_max, width_max);
    info.append(info_particular);
    EVT_CameraSetUInt32Param(camera, "Height", he);
    EVT_CameraSetUInt32Param(camera, "Width", we);
    info_particular.sprintf("Set Res: %d x %d \n", he, we);
    info.append(info_particular);
    EVT_CameraSetUInt32Param(camera, "FrameRate", frame_rate);
    EVT_CameraGetUInt32Param(camera, "FrameRate", &frame_rate_status);
    EVT_CameraGetUInt32ParamMax(camera, "FrameRate", &frame_rate_max);
    EVT_CameraSetUInt32Param(camera, "Gain", GAIN_VAL);
    EVT_CameraSetUInt32Param(camera, "Exposure", exposition);
    EVT_CameraSetUInt32Param(camera, "OffsetX", offset_x);
    EVT_CameraSetUInt32Param(camera, "OffsetY", offset_y);
    strcpy_s(friendName,deviceInfo[info_static].nic.friendlyName);
    info_particular.sprintf("Network Name: %s\n",friendName);
    info.append(info_particular);
    strcpy_s(ip,deviceInfo[info_static].nic.ip4Address);
    info_particular.sprintf("Ip: %s\n",ip);
    info.append(info_particular);
    QString max_fps_str =  QString("%1").arg(frame_rate_max);
    ui->label_fps_max->setText(max_fps_str);
    if (info_static == 0){ui->info1->setText(info);}
    else if (info_static == 1){ui->info2->setText(info);}
    else if (info_static == 2){ui->info3->setText(info);}
    else if (info_static == 3){ui->info4->setText(info);}
}

int getW(CEmergentCamera* camera) {
    unsigned int width;
    EVT_CameraGetUInt32Param(camera, "Width", &width);
    return width;
}

int getH(CEmergentCamera* camera) {
    unsigned int heigth;
    EVT_CameraGetUInt32Param(camera, "Height", &heigth);
    return heigth;
}

int getmaxW(CEmergentCamera* camera) {
    unsigned int width;
    EVT_CameraGetUInt32ParamMax(camera, "Width", &width);
    return width;
}

int getmaxH(CEmergentCamera* camera) {
    unsigned int heigth;
    EVT_CameraGetUInt32ParamMax(camera, "Height", &heigth);
    return heigth;
}

int getMaxGain(CEmergentCamera* camera){
    unsigned int param_val_max;
    EVT_CameraGetUInt32ParamMax(camera, "Gain", &param_val_max);
    return param_val_max;
}

int getMinGain(CEmergentCamera* camera){
    unsigned int param_val_min;
    EVT_CameraGetUInt32ParamMin(camera, "Gain", &param_val_min);
    return param_val_min;
}

int getMaxExposure(CEmergentCamera* camera){
    unsigned int param_val_max;
    EVT_CameraGetUInt32ParamMax(camera, "Exposure", &param_val_max);
    return param_val_max;
}

int getMinExposure(CEmergentCamera* camera){
    unsigned int param_val_min;
    EVT_CameraGetUInt32ParamMin(camera, "Exposure", &param_val_min);
    return param_val_min;
}

