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
    void finish_download_file(bool success,const QString& message, const QString& save_file_path);

    void finish_upload_file(bool success,const QString& message);
public slots:
    void download_large_file(const QString& file_path, const QString& save_file_path);

    // void upload_file(const QString& file_path, const QString& upload_dir);
};
}   // namespace client
}   // namespace tang