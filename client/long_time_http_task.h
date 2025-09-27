#pragma once
#include <QNetworkAccessManager>

namespace tang {
namespace client {
class LongtimeHttpTask : public QObject {
    Q_OBJECT
private:
    QNetworkAccessManager* manager;

public:
    LongtimeHttpTask();
    ~LongtimeHttpTask();

signals:
    void finish_download_file(bool success, const QString& message, const QString& save_file_path);
    // for single file
    void finish_upload_files(int n_success, int n_failed);
    // for multi files!
    void download_percent(int percent);
    //0~100
    void upload_percent(int percent);
public slots:
    void download_large_file(const QString& src_file_path, const QString& save_file_path);

    void upload_files(const QStringList& upload_files, const QString& dst_dir);
};
}   // namespace client
}   // namespace tang