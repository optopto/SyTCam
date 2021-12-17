#-------------------------------------------------
#
# Project created by QtCreator 2021-05-23T20:52:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mTur
TEMPLATE = app
QT += serialport

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    createDir.cpp \
    exp_gain.cpp \
    focus_iris.cpp \
        main.cpp \
        mainwindow.cpp \
    cameraConfig.cpp \
    open_cam_thread.cpp \
    open_cameras.cpp \
    radioButton.cpp

HEADERS += \
        globalVar.h \
        mainwindow.h

FORMS += \
        mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/release/ -lEmergentCamera
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/debug/ -lEmergentCamera
else:unix: LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/ -lEmergentCamera

INCLUDEPATH += $$PWD/../../../../../opt/EVT/eSDK/include
DEPENDPATH += $$PWD/../../../../../opt/EVT/eSDK/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/release/ -lEmergentCameraC
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/debug/ -lEmergentCameraC
else:unix: LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/ -lEmergentCameraC

INCLUDEPATH += $$PWD/../../../../../opt/EVT/eSDK/include
DEPENDPATH += $$PWD/../../../../../opt/EVT/eSDK/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/release/ -lEmergentGenICam
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/debug/ -lEmergentGenICam
else:unix: LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/ -lEmergentGenICam

INCLUDEPATH += $$PWD/../../../../../opt/EVT/eSDK/include
DEPENDPATH += $$PWD/../../../../../opt/EVT/eSDK/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/release/ -lEmergentGigEVision
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/debug/ -lEmergentGigEVision
else:unix: LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/ -lEmergentGigEVision

INCLUDEPATH += $$PWD/../../../../../opt/EVT/eSDK/include
DEPENDPATH += $$PWD/../../../../../opt/EVT/eSDK/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/release/ -lmva
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/debug/ -lmva
else:unix: LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/ -lmva

INCLUDEPATH += $$PWD/../../../../../opt/EVT/eSDK/include
DEPENDPATH += $$PWD/../../../../../opt/EVT/eSDK/include


INCLUDEPATH += /usr/local/include/opencv4

LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_imgcodecs -lopencv_videoio

LIBS += -L$$PWD/LibSSH/lib -lssh

CUDA_SOURCE += cuda_test.cu

CUDA_DIR = /usr/local/cuda
INCLUDEPATH  += $$CUDA_DIR/include
QMAKE_LIBDIR += $$CUDA_DIR/lib64
LIBS += -lcudart -lcuda -lcufft -lnvidia-ml
CUDA_ARCH = sm_61                # Yeah! I've a new device. Adjust with your compute capability
# Here are some NVCC flags I've always used by default.
NVCCFLAGS     = --compiler-options -fno-strict-aliasing -use_fast_math --ptxas-options=-v
CUDA_INC = $$join(INCLUDEPATH,' -I','-I',' ')
cuda.commands = $$CUDA_DIR/bin/nvcc -m64 -O3 -arch=$$CUDA_ARCH -c $$NVCCFLAGS \
                $$CUDA_INC $$LIBS  ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT} \
                2>&1 | sed -r \"s/\\(([0-9]+)\\)/:\\1/g\" 1>&2
cuda.dependency_type = TYPE_C
cuda.depend_command = $$CUDA_DIR/bin/nvcc -O3 -M $$CUDA_INC $$NVCCFLAGS ${QMAKE_FILE_NAME}| sed \"s/^.*: //\"
cuda.input = CUDA_SOURCE
cuda.output = $${OBJECTS_DIR}/${QMAKE_FILE_BASE}$${QMAKE_EXT_OBJ}
QMAKE_EXTRA_COMPILERS += cuda ```
