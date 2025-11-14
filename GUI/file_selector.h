#ifndef FILE_SELECTOR_H
#define FILE_SELECTOR_H

#include <QDialog>

namespace Ui {
class File_Selector;
}

class File_Selector : public QDialog
{
    Q_OBJECT

public:
    explicit File_Selector(QWidget *parent = nullptr, bool c = false);
    ~File_Selector();

private slots:
    void on_selectFileButton_clicked();

    void on_actionButton_clicked();

    void on_backButton_clicked();

private:
    Ui::File_Selector *ui;
    bool compress;
    QString currentFilePath;
    QString currentFileContent;
};

#endif // FILE_SELECTOR_H
