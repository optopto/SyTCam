#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_actionSave_Directory_triggered()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory("/home/pc");
    if (dialog.exec()){save_directory = dialog.selectedFiles();}
    ui->label_dir_save->setText(save_directory[0]);

}

void MainWindow::createQtdir(int frame_rate, int camM){
    exist = save_directory[0];
    QString cameras; QString name_dirfps;
    int month = QDate::currentDate().month();
    int day = QDate::currentDate().day();
    int hour  = QTime::currentTime().hour();
    int minutes = QTime::currentTime().minute();
    int sec = QTime::currentTime().second();
    name_dirfps = QString("/%1_FPS_%2%3_%4%5-%6").arg(frame_rate).arg(day).arg(month).arg(hour).arg(minutes).arg(sec);
    exist =  exist.append(name_dirfps);
    if(QDir(exist).exists()){
        qDebug() << "El directorio existe";
    }else{
        QDir().mkdir(exist);
        qDebug() << "Creando Directorio";
        exist.append("/camera_");
        for(int cam = 1; cam <= camM; cam++){
            cameras = QString(exist + "%1").arg(cam);
            QDir().mkdir(cameras);
        }
        qDebug() << "Directorios Creados";
    }
}

void MainWindow::on_actionClose_triggered()
{
    done = false;
}

