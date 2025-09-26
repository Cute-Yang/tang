#pragma once
#include "long_time_http_task.h"
#include "client_singleton.h"
#include <QFile>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QJsonObject>
#include <QThread>




namespace tang {
namespace client {
LongtimeHttpTask::LongtimeHttpTask()
    : manager(new QNetworkAccessManager(this)) {}
LongtimeHttpTask::~LongtimeHttpTask() {}

void LongtimeHttpTask::download_large_file(const QString& file_path,
                                           const QString& save_file_path) {
    // test long long time!
    // qDebug() << "Start download file...............";
    // qDebug() << "maybe cost 8 sec...";
    QUrl url(ClientSingleton::get_http_urls_instance().get_download_file_url(), QUrl::StrictMode);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // prepare the json data!
    QJsonObject json_data;
    json_data["file_path"] = file_path;
    QJsonDocument  json_doc(json_data);
    QByteArray     json_bytes = json_doc.toJson(QJsonDocument::Compact);
    QNetworkReply* reply      = manager->post(request, json_bytes);
    auto&          cache_dir  = ClientSingleton::get_cache_file_dir();
    if (cache_dir.isEmpty()) {
        emit finish_download_file(false, QString("缓存目录无效 😮😮😮..."), save_file_path);
        return;
    }
    if (QFile::exists(save_file_path)) {
        if (!QFile::remove(save_file_path)) {
            emit finish_download_file(
                false, QString("无法删除过时的缓存文件 🤣🤣🤣..."), save_file_path);
            return;
        }
    }

    QFile* writer = new QFile(file_path);
    if (!writer->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        emit finish_download_file(false, QString("无法创建缓存文件... 😫😫😫"), save_file_path);
        return;
    }
    // then cache it!
    // auto reply = this->send_download_file_req(file_name);
    // write by chunk!
    connect(reply, &QNetworkReply::readyRead, this, [writer, reply, this]() {
        // allocate buffer with 16 * 1024!
        constexpr size_t                    write_buffer_size = 16 * 1024;
        std::array<char, write_buffer_size> buffer;
        while (true) {
            auto n_read_bytes = reply->read(buffer.data(), buffer.size());
            if (n_read_bytes > 0) {
                writer->write(buffer.data(), n_read_bytes);
            } else {
                break;
            }
        }
    });

    // when finished!
    connect(reply, &QNetworkReply::finished, this, [writer, reply, save_file_path, this]() {
        if (reply->error() != QNetworkReply::NoError) {
            reply->deleteLater();
            emit finish_download_file(false, QString("文件下载失败😫😫😫..."), save_file_path);
            return;
        }
        writer->flush();
        writer->close();
        writer->deleteLater();
        emit finish_download_file(true, QString("下载成功😊😊😊..."), save_file_path);
    });

    // report the progress!
    connect(reply, &QNetworkReply::downloadProgress, this, [this](qint64 n_recv, qint64 n_total) {
        int  percent = static_cast<int>(n_recv * 100.0 / n_total);
        emit download_percent(percent);
    });
}

void LongtimeHttpTask::upload_file(const QString& src_file_path, const QString& save_dir) {
    QHttpMultiPart* multi_part = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    // add params!
    QFile* file = new QFile(src_file_path);
    if (!file->open(QIODevice::ReadOnly)) {
        delete file;
        emit finish_upload_file(false, QString("无法打开文件😫😫😫"));
        return;
    }

    QHttpPart text_part;
    text_part.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QString("form-data; name=\"save_dir\""));
    multi_part->append(text_part);

    // support multi file then
    QHttpPart file_part;
    file_part.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"upload_file\"; filename=\"" +
                                 QFileInfo(*file).fileName() + "\""));
    file_part.setBodyDevice(file);   // 将文件流交给 filePart
    file->setParent(multi_part);     // 确保 file 的生命周期由 multiPart 管理
    multi_part->append(file_part);

    QUrl            url(ClientSingleton::get_http_urls_instance().get_download_file_url());
    QNetworkRequest request(url);
    QNetworkReply*  reply = manager->post(request, multi_part);

    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit finish_upload_file(false, reply->errorString());
        }
        emit finish_upload_file(true, "上传成功😁😁😁...");
    });
}

}   // namespace client
}   // namespace tang