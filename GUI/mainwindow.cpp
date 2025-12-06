#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "..\driver.cpp"  // Include your Huffman class

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Huffman File Compressor");
    setAcceptDrops(true);
    setupTables();

    // Initialize with both checked and total counts
    ui->txtSelLabel->setText("Selected: (0/0) Files\nto Compress");
    ui->daabSelLabel->setText("Selected: (0/0) Files\nto Decompress");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTables()
{
    // Setup txtTable with a checkbox column
    ui->txtTable->setColumnCount(4);
    ui->txtTable->setHorizontalHeaderLabels({"Select", "File Name", "File Path", "Size"});
    ui->txtTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->txtTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->txtTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->txtTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->txtTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->txtTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->txtTable->setAcceptDrops(false);
    ui->txtTable->setColumnWidth(0, 70);

    // Set object name for auto-connect (if not already set in UI designer)
    ui->txtTable->setObjectName("txtTable");

    // Setup daabTable with a checkbox column
    ui->daabTable->setColumnCount(4);
    ui->daabTable->setHorizontalHeaderLabels({"Select", "File Name", "File Path", "Size"});
    ui->daabTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->daabTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->daabTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->daabTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->daabTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->daabTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->daabTable->setAcceptDrops(false);
    ui->daabTable->setColumnWidth(0, 70);

    // Set object name for auto-connect (if not already set in UI designer)
    ui->daabTable->setObjectName("daabTable");
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (!mimeData->hasUrls()) {
        return;
    }

    for (const QUrl &url : mimeData->urls()) {
        QString filePath = url.toLocalFile();
        QFileInfo fileInfo(filePath);

        if (!fileInfo.exists() || !fileInfo.isFile()) {
            continue;
        }

        QString extension = fileInfo.suffix().toLower();

        FileData fileData;
        fileData.fileName = fileInfo.fileName();
        fileData.filePath = fileInfo.absoluteFilePath();
        fileData.fileSize = fileInfo.size();

        if (extension == "txt") {
            if (!isFileAlreadyAdded(m_txtFiles, fileData.filePath)) {
                m_txtFiles.append(fileData);
                addFileToTable(ui->txtTable, fileData);
            }
        }
        else if (extension == "daab") {
            if (!isFileAlreadyAdded(m_daabFiles, fileData.filePath)) {
                m_daabFiles.append(fileData);
                addFileToTable(ui->daabTable, fileData);
            }
        }
        else
        {
            QMessageBox::warning(this, "Wrong Files",
                                 "Only \".txt\" or \".daab\" Files.");
        }
    }

    event->acceptProposedAction();
}

void MainWindow::on_compressPushButton_clicked()
{
    QList<FileData> filesToCompress = getCheckedFilesFromTable(ui->txtTable, m_txtFiles);

    if (filesToCompress.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please check at least one .txt file to compress.");
        return;
    }

    int successCount = 0;
    int failCount = 0;
    QStringList failedFiles;

    for (const FileData &txtFile : filesToCompress) {
        std::string inputPath = txtFile.filePath.toStdString();

        // Call run_process function (true = compress)
        std::string outputPath = run_process(inputPath, true);

        if (!outputPath.empty()) {
            QString qOutputPath = QString::fromStdString(outputPath);
            QFileInfo outputInfo(qOutputPath);

            if (outputInfo.exists()) {
                FileData daabFile;
                daabFile.fileName = outputInfo.fileName();
                daabFile.filePath = outputInfo.absoluteFilePath();
                daabFile.fileSize = outputInfo.size();

                // Add to daab list and table (if not already there)
                if (!isFileAlreadyAdded(m_daabFiles, daabFile.filePath)) {
                    m_daabFiles.append(daabFile);
                    addFileToTable(ui->daabTable, daabFile);
                }
                successCount++;
            } else {
                failCount++;
                failedFiles.append(txtFile.fileName);
            }
        } else {
            failCount++;
            failedFiles.append(txtFile.fileName);
        }
    }

    QString message = QString("Compressed: %1 file(s)\nFailed: %2 file(s)").arg(successCount).arg(failCount);
    if (!failedFiles.isEmpty()) {
        message += "\n\nFailed files:\n" + failedFiles.join("\n");
    }

    if (failCount == 0) {
        QMessageBox::information(this, "Compression Complete", message);
    } else {
        QMessageBox::warning(this, "Compression Complete", message);
    }

    // Remove processed files from TXT table & list
    QList<QString> processedPaths;
    for (const FileData &txtFile : filesToCompress)
        processedPaths.append(txtFile.filePath);

    removeRowsByFilePaths(ui->txtTable, m_txtFiles, processedPaths);

    updateTxtSelectionCount();
    updateDaabSelectionCount();
}



void MainWindow::on_decompressPushButton_clicked()
{
    QList<FileData> filesToDecompress = getCheckedFilesFromTable(ui->daabTable, m_daabFiles);

    if (filesToDecompress.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please check at least one .daab file to decompress.");
        return;
    }

    int successCount = 0;
    int failCount = 0;
    QStringList failedFiles;

    for (const FileData &daabFile : filesToDecompress) {
        std::string inputPath = daabFile.filePath.toStdString();

        // Call run_process function (false = decompress)
        std::string outputPath = run_process(inputPath, false);

        if (!outputPath.empty()) {
            QString qOutputPath = QString::fromStdString(outputPath);
            QFileInfo outputInfo(qOutputPath);

            if (outputInfo.exists()) {
                FileData txtFile;
                txtFile.fileName = outputInfo.fileName();
                txtFile.filePath = outputInfo.absoluteFilePath();
                txtFile.fileSize = outputInfo.size();

                // Add to txt list and table (if not already there)
                if (!isFileAlreadyAdded(m_txtFiles, txtFile.filePath)) {
                    m_txtFiles.append(txtFile);
                    addFileToTable(ui->txtTable, txtFile);
                }
                successCount++;
            } else {
                failCount++;
                failedFiles.append(daabFile.fileName);
            }
        } else {
            failCount++;
            failedFiles.append(daabFile.fileName);
        }

        updateTxtSelectionCount();
        updateDaabSelectionCount();
    }

    QString message = QString("Decompressed: %1 file(s)\nFailed: %2 file(s)").arg(successCount).arg(failCount);
    if (!failedFiles.isEmpty()) {
        message += "\n\nFailed files:\n" + failedFiles.join("\n");
    }

    if (failCount == 0) {
        QMessageBox::information(this, "Decompression Complete", message);
    } else {
        QMessageBox::warning(this, "Decompression Complete", message);
    }

    // Remove processed files from DAAB table & list
    QList<QString> processedPaths;
    for (const FileData &daabFile : filesToDecompress)
        processedPaths.append(daabFile.filePath);

    removeRowsByFilePaths(ui->daabTable, m_daabFiles, processedPaths);


}

void MainWindow::addFileToTable(QTableWidget *table, const FileData &fileData)
{
    int row = table->rowCount();
    table->insertRow(row);

    // Create a checkable item for the first column
    QTableWidgetItem *checkItem = new QTableWidgetItem();
    checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    checkItem->setCheckState(Qt::Unchecked);
    table->setItem(row, 0, checkItem);

    // File name
    table->setItem(row, 1, new QTableWidgetItem(fileData.fileName));
    // File path
    table->setItem(row, 2, new QTableWidgetItem(fileData.filePath));
    // Size formatted
    table->setItem(row, 3, new QTableWidgetItem(formatFileSize(fileData.fileSize)));

    // Update selection count labels
    if (table == ui->txtTable) {
        updateTxtSelectionCount();
    } else if (table == ui->daabTable) {
        updateDaabSelectionCount();
    }
}


QString MainWindow::formatFileSize(qint64 size)
{
    if (size < 1024)
        return QString::number(size) + " B";
    else if (size < 1024 * 1024)
        return QString::number(size / 1024.0, 'f', 2) + " KB";
    else if (size < 1024 * 1024 * 1024)
        return QString::number(size / (1024.0 * 1024.0), 'f', 2) + " MB";
    else
        return QString::number(size / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
}

bool MainWindow::isFileAlreadyAdded(const QList<FileData> &list, const QString &path)
{
    for (const FileData &file : list) {
        if (file.filePath == path)
            return true;
    }
    return false;
}

QList<FileData> MainWindow::getCheckedFilesFromTable(QTableWidget *table, const QList<FileData> &sourceList)
{
    QList<FileData> checkedFiles;
    int rows = table->rowCount();

    for (int r = 0; r < rows; ++r) {
        QTableWidgetItem *checkItem = table->item(r, 0);
        if (checkItem && checkItem->checkState() == Qt::Checked) {
            // We assume sourceList is kept in the same row order as the table
            if (r >= 0 && r < sourceList.size())
                checkedFiles.append(sourceList.at(r));
            else {
                // Fallback: build FileData from table contents if sourceList is out-of-sync
                FileData f;
                f.fileName = table->item(r, 1) ? table->item(r,1)->text() : QString();
                f.filePath = table->item(r, 2) ? table->item(r,2)->text() : QString();
                QString sizeStr = table->item(r, 3) ? table->item(r,3)->text() : QString();
                f.fileSize = 0; // unknown when fallback
                checkedFiles.append(f);
            }
        }
    }
    return checkedFiles;
}

void MainWindow::removeRowsByFilePaths(QTableWidget *table, QList<FileData> &list, const QList<QString> &pathsToRemove)
{
    for (int i = table->rowCount() - 1; i >= 0; --i) {
        QString path = table->item(i, 2)->text(); // column 2 = file path
        if (pathsToRemove.contains(path)) {
            table->removeRow(i);
            // remove from QList
            for (int j = 0; j < list.size(); ++j) {
                if (list[j].filePath == path) {
                    list.removeAt(j);
                    break;
                }
            }
        }
    }
}

void MainWindow::on_removeTxtButton_clicked()
{
    QList<int> rowsToRemove;
    for (const auto &range : ui->txtTable->selectedRanges()) {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
            if (!rowsToRemove.contains(row))
                rowsToRemove.append(row);
        }
    }
    std::sort(rowsToRemove.begin(), rowsToRemove.end(), std::greater<int>());
    for (int row : rowsToRemove) {
        m_txtFiles.removeAt(row);
        ui->txtTable->removeRow(row);
    }
    updateTxtSelectionCount();  // Update label after removal
}

void MainWindow::on_removeDaabButton_clicked()
{
    QList<int> rowsToRemove;
    for (const auto &range : ui->daabTable->selectedRanges()) {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
            if (!rowsToRemove.contains(row))
                rowsToRemove.append(row);
        }
    }
    std::sort(rowsToRemove.begin(), rowsToRemove.end(), std::greater<int>());
    for (int row : rowsToRemove) {
        m_daabFiles.removeAt(row);
        ui->daabTable->removeRow(row);
    }
    updateDaabSelectionCount();  // Update label after removal
}

void MainWindow::on_txtTableClearPush_clicked()
{
    m_txtFiles.clear();
    ui->txtTable->setRowCount(0);
    updateTxtSelectionCount();  // Update label after clearing
}

void MainWindow::on_daabTableClearPush_clicked()
{
    m_daabFiles.clear();
    ui->daabTable->setRowCount(0);
    updateDaabSelectionCount();  // Update label after clearing
}

void MainWindow::on_txtTable_itemChanged(QTableWidgetItem *item)
{
    // Only count when checkbox column (column 0) changes
    if (item->column() == 0) {
        int checkedCount = 0;
        int totalCount = ui->txtTable->rowCount();

        for (int row = 0; row < totalCount; ++row) {
            QTableWidgetItem *checkItem = ui->txtTable->item(row, 0);
            if (checkItem && checkItem->checkState() == Qt::Checked) {
                checkedCount++;
            }
        }

        ui->txtSelLabel->setText(QString("Selected: (%1/%2) Files\nto Compress")
                                     .arg(checkedCount)
                                     .arg(totalCount));
    }
}

void MainWindow::on_daabTable_itemChanged(QTableWidgetItem *item)
{
    // Only count when checkbox column (column 0) changes
    if (item->column() == 0) {
        int checkedCount = 0;
        int totalCount = ui->daabTable->rowCount();

        for (int row = 0; row < totalCount; ++row) {
            QTableWidgetItem *checkItem = ui->daabTable->item(row, 0);
            if (checkItem && checkItem->checkState() == Qt::Checked) {
                checkedCount++;
            }
        }

        ui->daabSelLabel->setText(QString("Selected: (%1/%2) Files\nto Decompress")
                                      .arg(checkedCount)
                                      .arg(totalCount));
    }
}

void MainWindow::updateTxtSelectionCount()
{
    int checkedCount = 0;
    int totalCount = ui->txtTable->rowCount();

    for (int row = 0; row < totalCount; ++row) {
        QTableWidgetItem *checkItem = ui->txtTable->item(row, 0);
        if (checkItem && checkItem->checkState() == Qt::Checked) {
            checkedCount++;
        }
    }

    ui->txtSelLabel->setText(QString("Selected: (%1/%2) Files\nto Compress")
                                 .arg(checkedCount)
                                 .arg(totalCount));
}

void MainWindow::updateDaabSelectionCount()
{
    int checkedCount = 0;
    int totalCount = ui->daabTable->rowCount();

    for (int row = 0; row < totalCount; ++row) {
        QTableWidgetItem *checkItem = ui->daabTable->item(row, 0);
        if (checkItem && checkItem->checkState() == Qt::Checked) {
            checkedCount++;
        }
    }

    ui->daabSelLabel->setText(QString("Selected: (%1/%2) Files\nto Decompress")
                                  .arg(checkedCount)
                                  .arg(totalCount));
}
