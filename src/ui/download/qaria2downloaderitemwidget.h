#ifndef QARIA2DOWNLOADERITEMWIDGET_H
#define QARIA2DOWNLOADERITEMWIDGET_H

#include <QWidget>

#include "download/downloadfileinfo.h"

class QAria2Downloader;

namespace Ui {
class QAria2DownloaderItemWidget;
}

class QAria2DownloaderItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QAria2DownloaderItemWidget(QWidget *parent, QAria2Downloader *downloader);
    ~QAria2DownloaderItemWidget();

private slots:
    void onInfoChanged();
    void refreshStatus(int status);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadSpeed(qint64 download, qint64 upload);
    void on_pauseButton_clicked();
    void on_QAria2DownloaderItemWidget_customContextMenuRequested(const QPoint &pos);
    void on_actionShow_Downloaded_File_in_Folder_triggered();
    void on_actionCopy_Download_Link_triggered();

private:
    Ui::QAria2DownloaderItemWidget *ui;
    QAria2Downloader *downloader_;
};

#endif // QARIA2DOWNLOADERITEMWIDGET_H
