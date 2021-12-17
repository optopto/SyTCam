#include "mainwindow.h"
#include <QApplication>
#include <cuda_runtime.h>
#include <cuda.h>
#include <stdio.h>
#include "device_launch_parameters.h"


//void addVector(int* h, int* b, int* c);

extern "C" int callAdd();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("/home/pc/Escritorio/InterfazVersion_0_2/mTur64x64.png"));
    cudaDeviceProp prop;
    int res = 0;
    int gpu = 0;
    int dispositivos = 0;
    cudaGetDeviceCount(&dispositivos);
    if(dispositivos > 0)
        if(dispositivos == 1){ cout << "Hay " << dispositivos << " dispositivo compatible con CUDA conectado" << endl; }
        else{ cout << "Hay " << dispositivos << " dispositivos conectados" << endl; }
    else { cout << "No hay GPU compatibles con CUDA en el computador" << endl; }

    for (gpu = 0; gpu < dispositivos; gpu++)
    {
        cudaGetDeviceProperties(&prop, gpu);
        cout << "Modelo: " << prop.name << endl;
        cout << "Memoria Global: " << prop.totalGlobalMem/pow(10,9) <<" GB" << endl;
        cout << "Maxima memoria a compartir por bloques: " << prop.sharedMemPerBlock/pow(10,3) << " KB" << endl;
        cout << "WarpSize: " << prop.warpSize << endl;
        cout << "Mempitch: " << prop.memPitch/pow(10,9) << " GB" << endl;
        cout << "Max thread per block: " << prop.maxThreadsPerBlock << endl;
        cout << "Max thread per Dim: " << "[ ( x: " << prop.maxThreadsDim[0] << " ) , ( y: " << prop.maxThreadsDim[1] << " ) , ( z: " << prop.maxThreadsDim[2] << " ) ]" << endl;
        cout << "Max block each grid: " << "[ ( x: " << prop.maxGridSize[0] << " ) , ( y: " << prop.maxGridSize[1] << " ) , ( z: " << prop.maxGridSize[2] << " ) ]" << endl;
        cout << "Total constant memory: " << prop.totalConstMem << endl;
        cout << "Max compute compatibility: " << prop.major << ". Men compute compatibility: " << prop.minor << "." << endl;
        cout << "Multiprocesadores: " << prop.multiProcessorCount << endl;
        cout << "Kind compute: "    << prop.computeMode << endl;
    }

    //res = callAdd();
    nvmlDevice_t device_;
    //unsigned int dev = 0;
    const char* id = "00000000:82:00.0";
    const char* UUID = "GPU-d7c8fa1e-d805-ae13-59c5-c3ed749faab1";
    const char* serial = "1324620012940";
    nvmlDeviceGetHandleBySerial(serial,&device_);
    //nvmlDeviceGetHandleByUUID(UUID,&device);
    //nvmlDeviceGetHandleByPciBusId_v2(id,&device);
    //nvmlDeviceGetHandleByIndex_v2(dev,&device);
    nvmlBAR1Memory_t info;
    nvmlDeviceGetBAR1MemoryInfo(device_,&info);
    long long free = info.bar1Free;
    cout << free/1048576<< endl;
    cout << info.bar1Total/1048576<< endl;
    cout << info.bar1Used/1048576<< endl;
   // cuPointerGetAttribute()

    CUDA_POINTER_ATTRIBUTE_P2P_TOKENS test;
    cout << test.p2pToken << endl;
    cout << test.vaSpaceToken << endl;

    void *data;
    unsigned int flag = 0;
    CUdeviceptr pointer = 0;
    CUresult response;
    response = cuPointerSetAttribute(&flag,CU_POINTER_ATTRIBUTE_SYNC_MEMOPS,pointer);
    cout << response << endl;
    cout << flag << endl;
    cout << pointer << endl;

    uint64_t buf_id;
    response = cuPointerGetAttribute(&buf_id, CU_POINTER_ATTRIBUTE_BUFFER_ID, pointer);

    cout << response << endl;
    cout << buf_id << endl;
    cout << pointer << endl;


    w.show();
    return a.exec();

}


