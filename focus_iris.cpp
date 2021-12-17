#include <mainwindow.h>
#include <ui_mainwindow.h>


void MainWindow::on_aperturaButton_clicked()
{
    if(iris != 0){iris = iris - 1;}
    //char parameter[10];
    //sprintf(parameter,"A0%d#",iris);
    //configureLensFunc(parameter[9]);
    //controlUSB(iris);
}

void MainWindow::on_maperturaButton_clicked()
{
    QMessageBox alertaControl;
    if(iris < 9){iris = iris + 1;}
    //char parameter[10];
    //sprintf(parameter,"A0%d#",iris);
    //configureLensFunc(parameter[9]);
}

void MainWindow::on_resetFocButton_clicked()
{
    QMessageBox alertaControl;
    focus = 5000;
    //char parameter[10];
  //  sprintf(parameter,"M%d#",focus);
//    configureLensFunc(parameter[9]);
    //controlUSB(focus);

}

void MainWindow::on_focusMButton_3_clicked()
{
    QMessageBox alertaControl;
    if(focus < 11000){focus = focus + 500;}
  //  char parameter[10];
   // sprintf(parameter,"M%d#",focus);
   // configureLensFunc(parameter[9]);
   // controlUSB(focus);

}

void MainWindow::on_focusButton_clicked()
{
    QMessageBox alertaControl;
    if(focus > 1500){focus = focus - 500;}
    //char parameter[10];
    //sprintf(parameter,"M%d#",focus);
    //configureLensFunc(parameter[9]);
 //   controlUSB(focus);

}

void MainWindow::on_resetApButton_clicked()
{
    QMessageBox alertaControl;
    iris = 0;
    //char parameter[10];
    //sprintf(parameter,"A0%d#",iris);
    //configureLensFunc(parameter[9]);
}

/*void MainWindow::configureLensFunc(char parameter){
    QMessageBox alertaControl;
    if(done == true){
        if(camConfig_1 == true){
            if(lente_one->isWritable()){
                lente_one->write(&parameter);
            }
        }
        if(camConfig_2 == true){
            if(lente_two->isWritable()){
                lente_two->write(&parameter);
            }
        }
        if(camConfig_3 == true){
            if(lente_three->isWritable()){
                lente_three->write(&parameter);
            }
        }
        if(camConfig_4 == true){
            if(lente_four->isWritable()){
                lente_four->write(&parameter);
            }
        }
        if(camConfig_1 == false && camConfig_2 == false && camConfig_3 == false && camConfig_4 == false){
            alertaControl.setText("Seleccione un parametro valido");
            alertaControl.exec();
        }
    }

}
*/


/*int MainWindow::controlUSB(int valor){

    int ok;
    int opcion = 0;
    char command[200];
    char buffer[256];
    int nbytes;
    //int verbosity = SSH_LOG_PROTOCOL;
    //int port = 22;
    char *password = "123";
    sshPi = ssh_new();
    if (sshPi == NULL){cout << "Error en conexión SSH" << endl;}
    else{
        cout << "No hay error en conexión" << endl;
    }
    ssh_options_set(sshPi, SSH_OPTIONS_HOST, "pi@169.254.201.236");
    //ssh_options_set(sshPi, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

    ok = ssh_connect(sshPi);
    if (ok != SSH_OK)
    {
        fprintf(stderr, "Error connecting to localhost: %s\n",
                ssh_get_error(sshPi));
    }else{cout << "Conexión Correcta" << endl;}

    ok  = ssh_userauth_password(sshPi, NULL, password);
    if (ok != SSH_AUTH_SUCCESS)
    {
        fprintf(stderr, "Error authenticating with password: %s\n",
                ssh_get_error(sshPi));
        ssh_disconnect(sshPi);
        ssh_free(sshPi);
    }else{cout << "Contraseña correcta" << endl;}

        sshPiChannel = ssh_channel_new(sshPi);
        if (sshPiChannel == NULL)
          return SSH_ERROR;

        ok = ssh_channel_open_session(sshPiChannel);
        if (ok != SSH_OK)
        {
          ssh_channel_free(sshPiChannel);
          return ok;
        }

        //ok = ssh_channel_request_exec(sshPiChannel, "cd Documents/controlLens/build-lensControl-Desktop-Debug && ls -l");
        sprintf(command,"sudo ./Documents/controlLens/build-lensControl-Desktop-Debug/lensControl %d %d",valor,opcion);
        cout << command << endl;
        ok = ssh_channel_request_exec(sshPiChannel,command);
        if (ok != SSH_OK)
        {
          ssh_channel_close(sshPiChannel);
          ssh_channel_free(sshPiChannel);
          return ok;
        }
        nbytes = ssh_channel_read_timeout(sshPiChannel, buffer, sizeof(buffer), 0,2000);

        while (nbytes > 0)
        {
          if (write(1, buffer, nbytes) != (unsigned int) nbytes)
          {
            ssh_channel_close(sshPiChannel);
            ssh_channel_free(sshPiChannel);
            return SSH_ERROR;
          }
          nbytes = ssh_channel_read(sshPiChannel, buffer, sizeof(buffer), 0);
          cout << nbytes << endl;

        }


        if (nbytes < 0)
        {
          ssh_channel_close(sshPiChannel);
          ssh_channel_free(sshPiChannel);
          return SSH_ERROR;
        }

//    ssh_channel_send_eof(sshPiChannel);
    ssh_channel_close(sshPiChannel);
    ssh_channel_free(sshPiChannel);
    ssh_disconnect(sshPi);
    ssh_free(sshPi);

    return 0;
}
*/
