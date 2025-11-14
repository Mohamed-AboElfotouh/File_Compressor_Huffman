#include "file_selector.h"
#include "ui_file_selector.h"
#include "../driver.cpp"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
File_Selector::File_Selector(QWidget *parent, bool c)
    : QDialog(parent),
    ui(new Ui::File_Selector),
    compress(c)
{
    ui->setupUi(this);
    if (compress) {
        setWindowTitle("Compress File");
        ui->selectLabel->setText("Select the file you need to compress:");
        ui->actionButton->setText("Compress");
    } else {
        setWindowTitle("Decompress File");
        ui->selectLabel->setText("Select the file you need to decompress:");
        ui->actionButton->setText("Decompress");
    }
}

File_Selector::~File_Selector()
{
    delete ui;
}


void File_Selector::on_selectFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QString(),  // Start directory (empty = current)
        tr("Text Files (*.txt);;All Files (*)")  // File filters
        );

    // Check if user selected a file
    if (fileName.isEmpty())
        return;

    currentFilePath = fileName;


    // Read the file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file:" << fileName;
        return;
    }


    QString c = "compress";
    if(!compress)
    {
        c = "decompress";
    }
    ui->selectLabel->setText("Select the file you need to " + c + ": " + fileName);
}


void File_Selector::on_actionButton_clicked()
{
    QString message = "";
    if(compress)
    {
        string new_filename = run_process(currentFilePath.toStdString(), true);
        QString qnew_filename = QString::fromStdString(new_filename);
        message = "The file has been compressed to " + qnew_filename + "!";
    }
    else
    {
        string new_filename = run_process(currentFilePath.toStdString(), false);
        QString qnew_filename = QString::fromStdString(new_filename);
        message = "The file has been decompressed to " + qnew_filename + "!";
    }
    QMessageBox::information(this, "Success!", message);
    exit(0);
}


void File_Selector::on_backButton_clicked()
{
    this->close();
    parentWidget()->show();
}

