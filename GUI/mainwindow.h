#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QUrl>
#include <QList>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Structure to store file information
struct FileData {
    QString fileName;
    QString filePath;
    qint64 fileSize;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Getters for file lists (for processing later)
    QList<FileData> getTxtFiles() const { return m_txtFiles; }
    QList<FileData> getDaabFiles() const { return m_daabFiles; }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;


private slots:
    void on_compressPushButton_clicked();
    void on_decompressPushButton_clicked();

    void on_txtTable_cellClicked(int row, int column);
    void on_daabTable_cellClicked(int row, int column);
    // Optional: slots for removing selected items
    void on_removeTxtButton_clicked();
    void on_removeDaabButton_clicked();

    void on_txtTableClearPush_clicked();

    void on_daabTableClearPush_clicked();

    void updateTxtSelectionCount();
    void updateDaabSelectionCount();


    void on_txtTable_itemChanged(QTableWidgetItem *item);

    void on_daabTable_itemChanged(QTableWidgetItem *item);

private:
    Ui::MainWindow *ui;

    // Storage for file data
    QList<FileData> m_txtFiles;
    QList<FileData> m_daabFiles;

    // Helper methods
    void setupTables();
    void addFileToTable(QTableWidget *table, const FileData &fileData);
    void refreshTxtTable();
    void refreshDaabTable();
    QString formatFileSize(qint64 size);
    bool isFileAlreadyAdded(const QList<FileData> &list, const QString &path);
    QList<FileData> getCheckedFilesFromTable(QTableWidget *table, const QList<FileData> &sourceList);
    void removeRowsByFilePaths(QTableWidget *table, QList<FileData> &list, const QList<QString> &pathsToRemove);

    QPoint m_StartPos;  // starting point for drag detection
    void startDragFromTable(QTableWidget *table, QMouseEvent *event);
    void displayFileContent(const QString &filePath);

};

#endif // MAINWINDOW_H
