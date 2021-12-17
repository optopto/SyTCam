#include <cuda_runtime.h>
#include <math.h>
#include <iostream>
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
//#include "mainwindow.h"
//#include "ui_mainwindow.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/viz/types.hpp>


#include <EmergentCameraAPIs.h>
#include <emergentframe.h>
#include <EvtParamAttribute.h>
#include <gigevisiondeviceinfo.h>
#include <EmergentFrameSave.h>
#include <EmergentAVIFile.h>

#include <cufft.h>
#include <cufftXt.h>
#include <cufftw.h>
#include <cuda.h>


using namespace std;
using namespace cv;
using namespace Emergent;

extern "C" int callAdd();
//extern "C" void testTwo();
extern "C" void getFrameCam(uchar *Frame,uchar *Frame_2,uchar *Frame_3,uchar *Frame_4 ,int buffer);
extern "C" void testCuda_(CEmergentCamera *camera, CEmergentFrame *evtFrame);

#define BUF 616*800
#define N 20


/*__host__ void testParam(){

    void *data;
    unsigned int flag = 1;
    CUdeviceptr pointer = 0;
    CUresult response;
    response = cuPointerSetAttribute(&flag,CU_POINTER_ATTRIBUTE_SYNC_MEMOPS,pointer);



    uint64_t buf_id = 0;
    response = cuPointerGetAttribute(&buf_id, CU_POINTER_ATTRIBUTE_BUFFER_ID, pointer);

}*/

__global__ void cudatxt(uchar *frame, uchar *frame_2, uchar *frame_3 ,uchar *frame_4, uchar *imagen){
    int j = threadIdx.x + blockIdx.x * blockDim.x;
if(j < BUF){
    imagen[j] = frame[j]+frame_2[j]+frame_3[j]+frame_4[j];
}

}

__global__ void getFrames(CEmergentCamera *cam, CEmergentFrame *evtF, uchar *imagen){
#ifndef __CUDA_ARCH__

    int j = threadIdx.x + blockIdx.x * blockDim.x;

   // if(j == 0){
    //   EVT_CameraGetFrame(cam,evtF,EVT_INFINITE);

    //}

    if(j < BUF){
    imagen[j] = evtF->imagePtr[j];
    }


#else

//comentario
#endif

}


void getFrameCam(uchar *Frame,uchar *Frame_2,uchar *Frame_3,uchar *Frame_4, int buffer){
    //srand (time(NULL));

    Mat mat;
    uchar * dev_frame;
    uchar * dev_frame_2;
    uchar * dev_frame_3;
    uchar * dev_frame_4;
    uchar * dev_img;
    uchar imagen[616][800];
//    uchar ranIm[616][800];

  /*  for(int i = 0; i < 616; i++){
        for(int j = 0; j<800;j++){
            ranIm[i][j] =  rand() % 2;
        }
    }*/

 /*   FILE *saveOriginal;
    saveOriginal = fopen("/home/pc/Documentos/matlab_/cuda_original.txt","w");
    for(int i = 0; i < 616; i++){
        for(int j = 0; j<800;j++)
            //       imagen2D[i][j] = Frame[j+(800*i)];
            fprintf(saveOriginal,"%d ", Frame[j+(800*i)]);
    }

    fclose(saveOriginal);*/

    cudaMalloc((void**)&dev_frame, buffer*sizeof(uchar));
    cudaMalloc((void**)&dev_frame_2, buffer*sizeof(uchar));
    cudaMalloc((void**)&dev_frame_3, buffer*sizeof(uchar));
    cudaMalloc((void**)&dev_frame_4, buffer*sizeof(uchar));
    cudaMalloc((void**)&dev_img, buffer*sizeof(uchar));
    cudaMemcpy(dev_frame,Frame,buffer*sizeof(uchar),cudaMemcpyHostToDevice);
    cudaMemcpy(dev_frame_2,Frame_2,buffer*sizeof(uchar),cudaMemcpyHostToDevice);
    cudaMemcpy(dev_frame_3,Frame_3,buffer*sizeof(uchar),cudaMemcpyHostToDevice);
    cudaMemcpy(dev_frame_4,Frame_4,buffer*sizeof(uchar),cudaMemcpyHostToDevice);
    cudaMemcpy(dev_img,imagen,buffer*sizeof(uchar),cudaMemcpyHostToDevice);
    cudatxt<<<800,616>>>(dev_frame, dev_frame_2, dev_frame_3, dev_frame_4, dev_img);
    cudaMemcpy(imagen,dev_img,buffer*sizeof(uchar),cudaMemcpyDeviceToHost);

   /* FILE *fileSave;
    fileSave = fopen("/home/pc/Documentos/matlab_/cuda.txt","w");
    for(int i = 0; i< 616; i++){
        for(int j = 0; j< 800; j++){
            fprintf(fileSave,"%d ", imagen[i][j]);
        }
    }

    fclose(fileSave);
*/
    /*mat = Mat(616, 800, CV_8UC1, imagen);
    namedWindow("cuda imagen", 1);
    imshow("cuda imagen",mat);
    waitKey(30);*/
    mat = Mat(616, 800, CV_8UC1, imagen);
    imwrite("/home/pc/Documentos/matlab_/sumaImagenes.tiff",mat);
   // imshow("procesada",mat);
   // waitKey(30)
    cudaFree(dev_frame);
    cudaFree(dev_frame_2);
    cudaFree(dev_frame_3);
    cudaFree(dev_frame_4);
    cudaFree(dev_img);
    return;
}

void testCuda_(CEmergentCamera* camera, CEmergentFrame *evtFrame){
    cout << "1" << endl;

    CEmergentCamera *gpuCam;
    CEmergentFrame *gpuFrame;
    uchar * gpuImagen;
    uchar imagen[BUF];
    cout << "2" << endl;

    cudaMalloc((void**)&gpuCam, sizeof(CEmergentCamera));
    cudaMalloc((void**)&gpuFrame, sizeof(CEmergentFrame));
    cudaMalloc((void**)&gpuImagen, BUF*sizeof(uchar));
    cout << "3" << endl;

    cudaMemcpy(gpuCam,camera,sizeof(CEmergentCamera),cudaMemcpyHostToDevice);
    cudaMemcpy(gpuFrame,evtFrame,sizeof(CEmergentFrame),cudaMemcpyHostToDevice);
    cudaMemcpy(gpuImagen,imagen,BUF*sizeof(uchar),cudaMemcpyHostToDevice);
    cout << "4" << endl;

    getFrames<<<800,616>>>(gpuCam,gpuFrame,gpuImagen);
    cout << "5" << endl;

    cudaMemcpy(imagen,gpuImagen,BUF*sizeof(uchar),cudaMemcpyDeviceToHost);
    cout << "6" << endl;

    FILE *save;
    save = fopen("/home/pc/Documentos/matlab_/prueba.txt","w");
    for(int  i = 0; i < BUF ; i++){
    //cout << (int)imagen[i] << endl;
    fprintf(save,"%d ", (int)imagen[i]);
    }
    fclose(save);
    cout << "7" << endl;

        Mat mat;
        mat = Mat(616, 800, CV_8UC1, imagen);
        imwrite("/media/pc/b889862b-2b8a-44b9-8595-8b3819d37d2b/imagenes/imagen_1.bmp",mat);


    cudaFree(gpuCam);
    cudaFree(gpuFrame);
    cudaFree(gpuImagen);


}

/*void testTwo(){

    testParam<<<1,1>>>();
}*/
