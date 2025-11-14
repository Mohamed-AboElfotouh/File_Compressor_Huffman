#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "file_selector.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Welcome!");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_compressPushButton_clicked()
{
    File_Selector *fileSelectorForm = new File_Selector(this, true);
    this->hide();
    fileSelectorForm->show();
}


void MainWindow::on_decompressPushButton_clicked()
{
    File_Selector *fileSelectorForm = new File_Selector(this, false);
    this->hide();
    fileSelectorForm->show();
}

