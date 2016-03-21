#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "externVar.h"

//extern const int g_T;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);


public slots:
    void stopRun();
    void resetValues();
    int run_main_program();
    void animateImages();
    void include_energy();
    void quit();
    void showHelp();
    void showAbout();
    void remove_Image_Files();
    void netburg();
    bool checkStop();
    void continueRun();
private:
    Ui::MainWindow ui;
};

#endif // MAINWINDOW_H
