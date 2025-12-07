#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "..\driver.cpp"  // Include your Huffman class
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QCheckBox>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_isDarkMode(false)  // Start with light mode
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setFixedSize(size());

    m_themeToggleBtn = new QPushButton("🌙 Dark Mode", this);
    m_themeToggleBtn->setFixedSize(120, 30);
    m_themeToggleBtn->raise();  // Bring to front

    ui->txtTable->raise();
    ui->daabTable->raise();

    ui->compressPushButton->raise();
    ui->decompressPushButton->raise();

    updateThemeButtonPosition();

    setWindowTitle("DAAB File Compressor");
    setWindowIcon(QIcon(":/icons/app_icon.ico"));
    setAcceptDrops(true);
    setupTables();

    connect(m_themeToggleBtn, &QPushButton::clicked,
            this, &MainWindow::toggleTheme);

    ui->txtSelLabel->setText("Selected: (0/0) Files\nto Compress");
    ui->daabSelLabel->setText("Selected: (0/0) Files\nto Decompress");

    setWindowIcon(QIcon(":/icons/DIcon.png"));

    // Apply initial theme
    applyLightTheme();

    QSettings settings("YourCompany", "DAABCompressor");
    m_isDarkMode = settings.value("darkMode", false).toBool();

    if (m_isDarkMode) {
        applyDarkTheme();
        m_themeToggleBtn->setText("☀️ Light Mode");
    } else {
        applyLightTheme();
        m_themeToggleBtn->setText("🌙 Dark Mode");
    }
}

void MainWindow::toggleTheme()
{
    m_isDarkMode = !m_isDarkMode;

    QSettings settings("YourCompany", "DAABCompressor");
    settings.setValue("darkMode", m_isDarkMode);

    if (m_isDarkMode) {
        applyDarkTheme();
        m_themeToggleBtn->setText("☀️ Light Mode");
    } else {
        applyLightTheme();
        m_themeToggleBtn->setText("🌙 Dark Mode");
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTables()
{
    ui->txtTable->setColumnCount(4);
    ui->txtTable->setHorizontalHeaderLabels({"", "File Name", "File Path", "Size"});
    ui->txtTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->txtTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->txtTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->txtTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->txtTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->txtTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->txtTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->txtTable->verticalHeader()->setVisible(false);
    ui->txtTable->setAcceptDrops(false);
    ui->txtTable->setColumnWidth(0, 40);
    ui->txtTable->setObjectName("txtTable");

    // Create "Select All" checkbox for txtTable header
    m_txtSelectAllCheckbox = new QCheckBox(ui->txtTable->horizontalHeader());
    m_txtSelectAllCheckbox->setToolTip("Select/Deselect All");
    connect(m_txtSelectAllCheckbox, &QCheckBox::stateChanged,
            this, &MainWindow::onTxtSelectAllChanged);

    ui->daabTable->setColumnCount(4);
    ui->daabTable->setHorizontalHeaderLabels({"", "File Name", "File Path", "Size"});
    ui->daabTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->daabTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->daabTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->daabTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->daabTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->daabTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->daabTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->daabTable->verticalHeader()->setVisible(false);
    ui->daabTable->setAcceptDrops(false);
    ui->daabTable->setColumnWidth(0, 40);
    ui->daabTable->setObjectName("daabTable");

    // Create "Select All" checkbox for daabTable header
    m_daabSelectAllCheckbox = new QCheckBox(ui->daabTable->horizontalHeader());
    m_daabSelectAllCheckbox->setToolTip("Select/Deselect All");
    connect(m_daabSelectAllCheckbox, &QCheckBox::stateChanged,
            this, &MainWindow::onDaabSelectAllChanged);

    // Position checkboxes
    updateHeaderCheckboxPositions();

    // Raise tables
    ui->txtTable->raise();
    ui->daabTable->raise();
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
                                 "Only \".txt\" or \".daab\" Files allowed.");
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
    table->blockSignals(true);

    int row = table->rowCount();
    table->insertRow(row);

    // Determine checkbox state based on header checkbox
    Qt::CheckState checkState = Qt::Unchecked;

    if (table == ui->txtTable) {
        if (m_txtSelectAllCheckbox->checkState() == Qt::Checked) {
            checkState = Qt::Checked;
        }
    } else if (table == ui->daabTable) {
        if (m_daabSelectAllCheckbox->checkState() == Qt::Checked) {
            checkState = Qt::Checked;
        }
    }

    // Create a checkable item for the first column
    QTableWidgetItem *checkItem = new QTableWidgetItem();
    checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    checkItem->setCheckState(checkState);
    table->setItem(row, 0, checkItem);

    // File name
    QTableWidgetItem *nameItem = new QTableWidgetItem(fileData.fileName);
    nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    table->setItem(row, 1, nameItem);

    // File path
    QTableWidgetItem *pathItem = new QTableWidgetItem(fileData.filePath);
    pathItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    table->setItem(row, 2, pathItem);

    // Size formatted
    QTableWidgetItem *sizeItem = new QTableWidgetItem(formatFileSize(fileData.fileSize));
    sizeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    table->setItem(row, 3, sizeItem);

    table->blockSignals(false);

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

void MainWindow::on_txtTable_cellClicked(int row, int column)
{
    if (row < 0) return;

    QTableWidgetItem *pathItem = ui->txtTable->item(row, 2);

    if (pathItem) {
        displayFileContent(pathItem->text());
    }
}

void MainWindow::on_daabTable_cellClicked(int row, int column)
{
    if (row < 0) return;

    QTableWidgetItem *pathItem = ui->daabTable->item(row, 2);

    if (pathItem) {
        displayFileContent(pathItem->text());
    }
}

void MainWindow::displayFileContent(const QString &filePath)
{
    QFile file(filePath);
    QFileInfo fileInfo(filePath);

    if (fileInfo.exists()) {
        ui->labelSizeSelectedFile->setText(QString::number(fileInfo.size()) + " bytes");
    } else {
        ui->labelSizeSelectedFile->setText("File not found");
        return;
    }

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();

        ui->textSelectedFile->setText(fileData);

        file.close();
    } else {
        ui->textSelectedFile->setText("Error: Could not open file for reading.");
    }
}

void MainWindow::applyLightTheme()
{
    qApp->setStyleSheet(R"(
        QMainWindow {
            background-color: #f5f5f5;
        }

        QGroupBox {
            font-weight: bold;
            border: 2px solid #cccccc;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
            background-color: #ffffff;
            color: #333333;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
            color: #333333;
        }

        QTableWidget {
            border: 1px solid #ddd;
            border-radius: 4px;
            background-color: #ffffff;
            color: #333333;
            gridline-color: #e0e0e0;
        }

        QTableWidget::item {
            padding: 5px;
            color: #333333;
        }

        QTableWidget::item:selected {
            background-color: #0078d4;
            color: white;
        }

        QHeaderView::section {
            background-color: #e8e8e8;
            color: #333333;
            padding: 5px;
            border: 1px solid #ddd;
            font-weight: bold;
        }

        QPushButton {
            background-color: #0078d4;
            font-size: 12px;
            color: white;
            border: none;
            padding: 0px 4px;
            border-radius: 4px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #1084d8;
        }

        QPushButton:pressed {
            background-color: #006cbd;
        }

        QPushButton:disabled {
            background-color: #88c4eb;
            color: #cccccc;
        }

        QLabel {
            color: #333333;
        }

        QMenuBar {
            background-color: #f0f0f0;
            color: #333333;
        }

        QMenuBar::item:selected {
            background-color: #0078d4;
            color: white;
        }

        QMenu {
            background-color: #ffffff;
            color: #333333;
            border: 1px solid #ccc;
        }

        QMenu::item:selected {
            background-color: #0078d4;
            color: white;
        }

        QTextEdit {
            background-color: #ffffff;
            color: #333333;
            border: 1px solid #ddd;
            border-radius: 4px;
            padding: 5px;
        }

        QMessageBox {
            background-color: #ffffff;
            color: #333333;
        }

        QMessageBox QLabel {
            color: #333333;
            font-size: 13px;
        }

        QMessageBox QPushButton {
            background-color: #0078d4;
            color: white;
            border: none;
            padding: 6px 20px;
            border-radius: 4px;
            min-width: 80px;
        }

        QMessageBox QPushButton:hover {
            background-color: #1084d8;
        }

        QMessageBox QPushButton:pressed {
            background-color: #006cbd;
        }
    )");
}

void MainWindow::applyDarkTheme()
{
    qApp->setStyleSheet(R"(
        QMainWindow {
            background-color: #1e1e1e;
        }

        QGroupBox {
            font-weight: bold;
            border: 2px solid #3c3c3c;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
            background-color: #2d2d2d;
            color: #e0e0e0;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
            color: #e0e0e0;
        }

        QTableWidget {
            border: 1px solid #3c3c3c;
            border-radius: 4px;
            background-color: #252526;
            color: #e0e0e0;
            gridline-color: #3c3c3c;
        }

        QTableWidget::item {
            padding: 5px;
            color: #e0e0e0;
        }

        QTableWidget::item:selected {
            background-color: #264f78;
            color: white;
        }

        QHeaderView::section {
            background-color: #333333;
            color: #e0e0e0;
            padding: 5px;
            border: 1px solid #3c3c3c;
            font-weight: bold;
        }

        QPushButton {
            background-color: #aaaaaa;
            color: #1e1e1e;
            border: none;
            padding: 0px 4px;
            border-radius: 4px;
            font-weight: bold;
            font-size: 12px;
        }

        QPushButton:hover {
            background-color: #bbbbbb;
        }

        QPushButton:pressed {
            background-color: #999999;
        }

        QPushButton:disabled {
            background-color: #666666;
            color: #888888;
        }

        QLabel {
            color: #e0e0e0;
        }

        QMenuBar {
            background-color: #2d2d2d;
            color: #e0e0e0;
        }

        QMenuBar::item:selected {
            background-color: #094771;
            color: white;
        }

        QMenu {
            background-color: #2d2d2d;
            color: #e0e0e0;
            border: 1px solid #3c3c3c;
        }

        QMenu::item:selected {
            background-color: #094771;
            color: white;
        }

        QScrollBar:vertical {
            background-color: #2d2d2d;
            width: 12px;
            border-radius: 6px;
        }

        QScrollBar::handle:vertical {
            background-color: #5a5a5a;
            border-radius: 6px;
            min-height: 20px;
        }

        QScrollBar::handle:vertical:hover {
            background-color: #787878;
        }

        QScrollBar:horizontal {
            background-color: #2d2d2d;
            height: 12px;
            border-radius: 6px;
        }

        QScrollBar::handle:horizontal {
            background-color: #5a5a5a;
            border-radius: 6px;
            min-width: 20px;
        }

        QTextEdit {
            background-color: #252526;
            color: #e0e0e0;
            border: 1px solid #3c3c3c;
            border-radius: 4px;
            padding: 5px;
        }

        QMessageBox {
            background-color: #2d2d2d;
            color: #e0e0e0;
        }

        QMessageBox QLabel {
            color: #e0e0e0;
            font-size: 13px;
        }

        QMessageBox QPushButton {
            background-color: #aaaaaa;
            color: #1e1e1e;
            border: none;
            padding: 6px 20px;
            border-radius: 4px;
            min-width: 80px;
        }

        QMessageBox QPushButton:hover {
            background-color: #bbbbbb;
        }

        QMessageBox QPushButton:pressed {
            background-color: #999999;
        }
    )");
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateThemeButtonPosition();
}

void MainWindow::updateThemeButtonPosition()
{
    // Position at top right with 10px margin
    int x = width() - m_themeToggleBtn->width() - 10;
    int y = ui->menubar->height() + 10;  // Below menu bar
    m_themeToggleBtn->move(x, y);
    m_themeToggleBtn->raise();  // Keep on top
}

void MainWindow::onTxtSelectAllChanged(int state)
{
    ui->txtTable->blockSignals(true);

    Qt::CheckState checkState = (state == Qt::Checked) ? Qt::Checked : Qt::Unchecked;

    for (int row = 0; row < ui->txtTable->rowCount(); ++row) {
        QTableWidgetItem *item = ui->txtTable->item(row, 0);
        if (item) {
            item->setCheckState(checkState);
        }
    }

    ui->txtTable->blockSignals(false);
    updateTxtSelectionCount();
}

void MainWindow::onDaabSelectAllChanged(int state)
{
    ui->daabTable->blockSignals(true);

    Qt::CheckState checkState = (state == Qt::Checked) ? Qt::Checked : Qt::Unchecked;

    for (int row = 0; row < ui->daabTable->rowCount(); ++row) {
        QTableWidgetItem *item = ui->daabTable->item(row, 0);
        if (item) {
            item->setCheckState(checkState);
        }
    }

    ui->daabTable->blockSignals(false);
    updateDaabSelectionCount();
}

void MainWindow::updateHeaderCheckboxPositions()
{
    // Position txtTable checkbox
    int txtHeaderHeight = ui->txtTable->horizontalHeader()->height();
    int txtColWidth = ui->txtTable->columnWidth(0);
    int checkboxSize = 20;

    int txtX = (txtColWidth - checkboxSize) / 2;
    int txtY = (txtHeaderHeight - checkboxSize) / 2;
    m_txtSelectAllCheckbox->setGeometry(txtX, txtY, checkboxSize, checkboxSize);

    // Position daabTable checkbox
    int daabHeaderHeight = ui->daabTable->horizontalHeader()->height();
    int daabColWidth = ui->daabTable->columnWidth(0);

    int daabX = (daabColWidth - checkboxSize) / 2;
    int daabY = (daabHeaderHeight - checkboxSize) / 2;
    m_daabSelectAllCheckbox->setGeometry(daabX, daabY, checkboxSize, checkboxSize);
}
