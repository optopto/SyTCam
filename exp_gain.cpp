#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_gainSlider_sliderMoved(int position)
{
    gain = position;
    QString gain_str = QString("%1").arg(gain);
    ui->lineEdit_gain->setText(gain_str);
    configureCamFunc();
}

void MainWindow::on_exposureSlider_sliderMoved(int position)
{
    exposure = position;
    QString exposure_str = QString("%1").arg(exposure);
    ui->lineEdit_exposure->setText(exposure_str);
    configureCamFunc();
}

void MainWindow::on_lineEdit_gain_returnPressed()
{
    QMessageBox alerta_param_i;
    gain = ui->lineEdit_gain->text().toInt();
    if(gain < global_gain_max  && gain > global_gain_min){
        ui->gainSlider->setValue(gain);
        configureCamFunc();
    }else{
        alerta_param_i.setText("Valor no valido");
        alerta_param_i.exec();
    }
}

void MainWindow::on_lineEdit_exposure_returnPressed()
{
    QMessageBox alerta_param_i;
    exposure = ui->lineEdit_exposure->text().toInt();
    if(exposure > global_expo_min && exposure<global_expo_max){
        ui->exposureSlider->setValue(exposure);
        configureCamFunc();
    }else{
        alerta_param_i.setText("Valor no valido");
        alerta_param_i.exec();
    }
}

void MainWindow::configureCamFunc(){
    QMessageBox alerta_param_i;
    if(done == true){
        if(camConfig_1 == true){
            configure_user(&camera[0], gain, 30, exposure, 0, global_roi_w_1, global_roi_h_1,global_roi_x_1,global_roi_y_1);
        }
        if(camConfig_2 == true){
            configure_user(&camera[1], gain, 30, exposure, 1, global_roi_w_2, global_roi_h_2,global_roi_x_2,global_roi_y_2);
        }
        if(camConfig_3 == true){
            configure_user(&camera[2], gain, 30, exposure, 2, global_roi_w_3, global_roi_h_3,global_roi_x_3,global_roi_y_3);
        }
        if(camConfig_4 == true){
            configure_user(&camera[3], gain, 30, exposure, 3, global_roi_w_4, global_roi_h_4,global_roi_x_4,global_roi_y_4);
        }
        if(camConfig_1 == false && camConfig_2 == false && camConfig_3 == false && camConfig_4 == false){
            alerta_param_i.setText("Seleccione un parametro valido");
            alerta_param_i.exec();
        }
    }
}


