#include "workspace_page.h"
#include "ElaMessageBar.h"
#include "client_global_config.h"
#include "client_singleton.h"
#include "common/response_keys.h"
#include "response_validator.h"
#include "util.h"
#include "workspace_page_ui.h"
#include "workspace_view_model.h"
#include <QBuffer>
#include <QFont>
#include <QFuture>
#include <QFutureWatcher>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSplitter>
#include <QStackedWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QStyledItemDelegate>
#include <QUrlQuery>
#include <QVBoxLayout>
#include <QtConcurrent/QtConcurrent>

using namespace tang::common;
namespace tang {
namespace client {

void add_test() {
    std::vector<QString> test_workspaces = {
        "唐远志", "lazydog", "Arics", "jack", "Moris", "Moon", "work"};
    // take care the life circle!
    std::vector<RemoteFileInfo> file_infos_data = {
        {"images", 4096, FileKind::kFolder, "2023-04-05 12:32:51"},
        {"report.docx", 1024, FileKind::kWord, "2023-04-05 12:32:51"},

        {"simulation.py", 1024, FileKind::kPython, "2021-05-05 13:32:51"},

        {"mat.cpp", 1024, FileKind::kCpp, "2023-04-05 12:32:51"},
        {"sunny", 1024, FileKind::kFolder, "2023-04-05 12:32:51"},
        {"show.ppt", 1024, FileKind::kPpt, "2023-04-05 12:32:51"},
        {"pay_for_month_9.pdf", 1024, FileKind::kPdf, "2023-04-05 12:32:51"},

        {"simulation.xls", 1024, FileKind::kExcel, "2021-05-05 13:32:51"},

        {"mat_gemm.cpp", 21305, FileKind::kCpp, "2023-04-05 12:32:51"},
        {"pipeline.txt", 1024, FileKind::kTxt, "2023-04-05 12:32:51"},
        {"works", 1024, FileKind::kFolder, "2023-04-05 12:32:51"},
        {"guassian_blur.cpp", 1024, FileKind::kCpp, "2023-04-05 12:32:51"},

        {"test_gradient.py", 1024, FileKind::kPython, "2021-05-05 13:32:51"},

        {"canny_test.cpp", 1024, FileKind::kCpp, "2023-04-05 12:32:51"},
        {"fish", 1024, FileKind::kFolder, "2023-04-05 12:32:51"},
        {"data_process", 1024, FileKind::kFolder, "2023-04-05 12:32:51"},
        {"transform.cpp", 1024, FileKind::kCpp, "2023-04-05 12:32:51"},
        {"merge_result.xls", 1024, FileKind::kExcel, "2021-05-05 13:32:51"}};

    // if you reset the data of cache worspace,remember to set the workspace names!because they
    // are views!
    auto& workspace_data_cache = ClientSingleton::get_cache_workspace_data_instance();
    workspace_data_cache.set_file_infos("唐远志", std::move(file_infos_data));
    workspace_data_cache.set_workspace_show_names(test_workspaces);
    workspace_data_cache.set_workspaces(test_workspaces);
}

RemoteWorkspacePage::RemoteWorkspacePage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new RemoteWorkspacePageUi())
    , path_helper()
    , pdf_displayer(new DisplayPdf())
    , show_widget(find_root_widget(this))
    , long_time_http_runner(new QThread(this))
    , long_time_http_worker(new LongtimeHttpTask())
    , new_dir_dialog(new NewDir())
    , right_menu(new WorkspaceRightMenu())
    , rename_file_dialog(new WorkspaceRenameFile())
    , delete_file_dialog(new WorkspaceDeleteFile()) {
    // reverse capacity
    ui->setup_ui(this);
    ui->workspace_content_list_view->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->workspace_content_table_view->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setTitleVisible(false);
    // this is not enabled!
    // this->setWindowTitle("workspace");
    // only for test
    add_test();
    // set models
    workspace_model = new RemoteWorkspaceInfoModel({}, ui->workspace_content_list_view);
    workspace_model->set_workspace_names(
        ClientSingleton::get_cache_workspace_data_instance().get_workspace_show_names());
    ui->workspace_view->setModel(workspace_model);

    const QStringList headers = {"icon", "名称", "类型", "大小", "修改时间"};
    file_info_table_model =
        new RemoteFileInfoViewModel(headers, {}, ui->workspace_content_table_view);
    file_info_table_model->set_file_infos(
        ClientSingleton::get_cache_workspace_data_instance().get_file_infos("唐远志"));
    ui->workspace_content_table_view->setModel(file_info_table_model);

    file_info_list_model = new RemoteFileInfoListViewModel({}, ui->workspace_content_list_view);
    file_info_list_model->set_file_infos(
        ClientSingleton::get_cache_workspace_data_instance().get_file_infos("唐远志"));
    ui->workspace_content_list_view->setModel(file_info_list_model);
    this->initialize_connects();

    // move long time task to worker!
    long_time_http_worker->moveToThread(long_time_http_runner);
    long_time_http_runner->start();
    this->new_dir_dialog->hide();
}

// should use the std::unique_ptr to manage them!
RemoteWorkspacePage::~RemoteWorkspacePage() {
    delete ui;
    delete pdf_displayer;
    delete new_dir_dialog;
    delete rename_file_dialog;
    delete delete_file_dialog;
    // important,if not do this,maybe crash!
    long_time_http_runner->quit();
    long_time_http_runner->wait();
}

void RemoteWorkspacePage::show_message(const QString& message, bool error) {
    if (error) {
        ElaMessageBar::warning(ElaMessageBarType::TopLeft,
                               "workspace",
                               message,
                               ClientGlobalConfig::error_show_time,
                               show_widget);
    } else {
        ElaMessageBar::success(ElaMessageBarType::TopLeft,
                               "workspace",
                               message,
                               ClientGlobalConfig::message_show_time,
                               show_widget);
    }
}

void RemoteWorkspacePage::adjust_workspace_content_view() {
    size_t width = this->ui->workspace_content_table_view->width();
    // the percent of each column is 0.1,0.35,0.15,0.15,0.25
    size_t w0 = static_cast<size_t>(width * 0.1);
    size_t w1 = static_cast<size_t>(width * 0.35);
    size_t w2 = static_cast<size_t>(width * 0.15);
    size_t w3 = static_cast<size_t>(width * 0.15);
    size_t w4 = static_cast<size_t>(width * 0.25);
    ui->workspace_content_table_view->setColumnWidth(0, w0);
    ui->workspace_content_table_view->setColumnWidth(1, w1);
    ui->workspace_content_table_view->setColumnWidth(2, w2);
    ui->workspace_content_table_view->setColumnWidth(3, w3);
    ui->workspace_content_table_view->setColumnWidth(4, w4);
}


void RemoteWorkspacePage::refresh_workspace_content_impl() {
    QString workspace_path = ui->directory_line_edit->text();
    if (workspace_path.isEmpty()) {
        show_message("workspace为空🤣🤣🤣...");
        return;
    }
    this->get_workspace_content_impl(true);
}

void RemoteWorkspacePage::set_workspace_content_icon_size_impl(QSize icon_size) {
    this->ui->workspace_content_table_view->setIconSize(icon_size);
    this->ui->workspace_content_list_view->setIconSize(icon_size);
}

void RemoteWorkspacePage::set_workspace_data(std::span<QString> workspace) {
    this->workspace_model->set_workspace_names(workspace);
    this->workspace_model->layoutChanged();
}

void RemoteWorkspacePage::on_flush_workspace_name_button_clicked() {
    if (!ui->show_all_workspaces->getIsToggled()) {
        show_message("仅显示私人workspace", false);
        auto& workspace_data_cache = ClientSingleton::get_cache_workspace_data_instance();
        workspace_data_cache.set_workspaces({public_workspace_name.data(), "9"});
        workspace_data_cache.set_workspace_show_names({public_workspace_name.data(), "周琳"});
        this->set_workspace_data(workspace_data_cache.get_workspace_show_names());
        return;
    }
    this->send_get_workspace_req();
};


void RemoteWorkspacePage::process_workspace_response(QNetworkReply* reply) {
    auto document = get_json_document(reply);
    if (!document) {
        show_message("网络异常😮😮😮...");
        return;
    }

    auto&       document_value = document.value();
    QJsonObject json_data      = document_value.object();
    if (json_data[PublicResponseJsonKeys::status_key].toInt() !=
        static_cast<int>(StatusCode::kSuccess)) {
        show_message(QString("获取失败 ψ(._. )> (reason:%1)")
                         .arg(json_data[PublicResponseJsonKeys::message_key].toString()));

        return;
    }
    if (!WorkspaceResJsonValidator::validate(json_data)) {
        show_message("返回格式错误😫😫😫...");
        return;
    }

    auto& cache_workspace_data = ClientSingleton::get_cache_workspace_data_instance();
    auto  workspaces_json      = json_data[WorkspaceResJsonKeys::workspaces_key].toArray();

    auto workspace_show_names_json =
        json_data[WorkspaceResJsonKeys::workspace_show_names_key].toArray();
    if (workspaces_json.size() != workspace_show_names_json.size()) {
        show_message("返回数据错误😫😫😫...");
        return;
    }

    auto  n                    = workspaces_json.size();
    auto& workspaces           = cache_workspace_data.workspaces;
    auto& workspace_show_names = cache_workspace_data.workspace_show_names;
    workspaces.resize(n);
    workspace_show_names.resize(n);
    for (size_t i = 0; i < n; ++i) {
        // rvalue?
        workspaces[i]           = workspaces_json[i].toString();
        workspace_show_names[i] = workspace_show_names_json[i].toString();
    }
    // then set new span!
    show_message("成功获取workspace ヾ(≧▽≦*)o...", false);
    this->set_workspace_data(cache_workspace_data.get_workspace_show_names());
}

void RemoteWorkspacePage::send_get_workspace_req() {
    QNetworkRequest request(ClientSingleton::get_http_urls_instance().get_workspace_url());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    // no need any params!
    auto&          manager = ClientSingleton::get_network_manager_instance();
    QNetworkReply* reply   = manager.get(request);
    show_message("正在查询服务器😴😴😴...", false);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        this->process_workspace_response(reply);
    });
}

bool RemoteWorkspacePage::process_workspace_content_response(QNetworkReply* reply) {
    auto document = get_json_document(reply);
    if (!document) {
        show_message("网络异常😒😒😒...");
        return false;
    }
    auto&       document_value = document.value();
    QJsonObject json_data      = document_value.object();
    if (json_data[PublicResponseJsonKeys::status_key].toInt() !=
        static_cast<int>(StatusCode::kSuccess)) {
        show_message(QString("获取失败😂😂😂 (reason:%1)")
                         .arg(json_data[PublicResponseJsonKeys::message_key].toString()));
        return false;
    }

    if (!WorkspaceContentResJsonValidator::validate(json_data)) {
        show_message("返回数据验证失败😒😒😒...");
        return false;
    }

    auto& cache_workspace_data = ClientSingleton::get_cache_workspace_data_instance();
    auto  file_infos_json      = json_data[WorkspaceContentResponse::file_infos_key].toArray();

    std::vector<RemoteFileInfo> file_infos;
    file_infos.resize(file_infos_json.size());
    for (size_t i = 0; i < file_infos_json.size(); ++i) {
        auto file_info_json = file_infos_json[i].toObject();

        auto& file_info     = file_infos[i];
        file_info.file_name = file_info_json[WorkspaceContentResponse::file_name_key].toString();
        file_info.file_size =
            static_cast<size_t>(file_info_json[WorkspaceContentResponse::file_size_key].toInt());
        file_info.file_type = static_cast<tang::common::FileKind>(
            file_info_json[WorkspaceContentResponse::file_type_key].toInt());
        file_info.modify_time =
            file_info_json[WorkspaceContentResponse::last_write_time_key].toString();
    }

    auto folder_path = path_helper.get_workspace_path();
    // append to cache!
    cache_workspace_data.set_file_infos(folder_path, std::move(file_infos));
    auto file_infos_view = cache_workspace_data.get_file_infos(folder_path);
    this->set_workspace_content_data(file_infos_view);
    show_message("获取workspace成功😊😊😊", false);
    // here need to design!
    return true;
}

void RemoteWorkspacePage::get_workspace_content_impl(bool refresh) {
    auto& workspace_cache = ClientSingleton::get_cache_workspace_data_instance();
    auto  folder_path     = this->path_helper.get_workspace_path();
    auto  file_infos      = workspace_cache.get_file_infos(folder_path);
    if (refresh || file_infos.empty()) {
        this->send_get_workspace_content_req(folder_path);
    } else {
        show_message("从缓存获取(you can refresh) 😊😊😊", false);
        ui->directory_line_edit->setText(folder_path);
        this->set_workspace_content_data(file_infos);
    }
}

void RemoteWorkspacePage::send_get_workspace_content_req(const QString&          folder_path,
                                                         std::function<void()>&& success_callback,
                                                         std::function<void()>&& failed_callback) {
    QNetworkRequest request(ClientSingleton::get_http_urls_instance().get_workspace_content_url());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    // no need any params!
    auto&     manager = ClientSingleton::get_network_manager_instance();
    QUrlQuery query;
    query.addQueryItem("folder_path", folder_path);
    QByteArray     query_data = query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkReply* reply      = manager.post(request, query_data);
    show_message("正在查询服务器😴😴😴...", false);
    connect(
        reply,
        &QNetworkReply::finished,
        this,
        [this, reply, c1 = std::move(success_callback), c2 = std::move(failed_callback)] {
            if (this->process_workspace_content_response(reply)) {
                if (c1) {
                    c1();
                }
                this->ui->directory_line_edit->setText(this->path_helper.get_workspace_show_path());
            } else {
                if (c2) {
                    c2();
                }
                this->path_helper.pop_back_safe();
            }
        });
}

void RemoteWorkspacePage::on_workspace_item_clicked(const QModelIndex& index) {
    if (!index.isValid()) {
        return;
    }
    int   row                  = index.row();
    int   col                  = index.column();
    auto& cache_workspace_data = ClientSingleton::get_cache_workspace_data_instance();
    auto  workspaces           = cache_workspace_data.get_workspaces();
    if (row < 0 || row >= workspaces.size()) {
        // out of index!
        return;
    }
    auto workspace_show_names = cache_workspace_data.get_workspace_show_names();

    auto workspace      = workspaces[row];
    auto workspace_name = workspace_show_names[row];
    show_message(QString("切换工作区 %1 (●'◡'●)").arg(workspace_name), false);
    path_helper.clear();
    path_helper.set_workspace(workspace);
    path_helper.set_workspace_show_name(workspace_name);
    ui->directory_line_edit->setText(path_helper.get_workspace_show_path());
    this->set_workspace_content_data({});
    this->get_workspace_content_impl(false);
}

void RemoteWorkspacePage::on_view_detail_button_clicked() {
    ui->stacked_workspace_content_widget->setCurrentWidget(ui->workspace_content_table_view);
}

void RemoteWorkspacePage::on_view_tiling_button_clicked() {
    ui->stacked_workspace_content_widget->setCurrentWidget(ui->workspace_content_list_view);
}

void RemoteWorkspacePage::set_workspace_content_data(std::span<RemoteFileInfo> file_infos) {
    this->file_info_list_model->set_file_infos(file_infos);
    this->file_info_list_model->layoutChanged();
    this->file_info_table_model->set_file_infos(file_infos);
    this->file_info_table_model->layoutChanged();
}

void RemoteWorkspacePage::enter_folder_impl(const QString& folder_name) {
    this->path_helper.append(folder_name);
    this->get_workspace_content_impl(false);
}

void RemoteWorkspacePage::workspace_content_double_clicked_impl(const RemoteFileInfo& file_info) {
    if (file_info.file_type == FileKind::kFolder) {
        this->enter_folder_impl(file_info.file_name);
    } else if (file_info.file_type == FileKind::kPdf) {
        this->display_pdf_impl(file_info.file_name, file_info.file_size);
    } else {
    }
}


void RemoteWorkspacePage::on_back_button_clicked() {
    this->path_helper.pop_back_safe();
    this->get_workspace_content_impl(false);
}

void RemoteWorkspacePage::on_adjust_content_view_button_clicked() {
    this->adjust_workspace_content_view();
}


QNetworkReply* RemoteWorkspacePage::send_download_file_req(const QString& file_name) {
    auto& manager = ClientSingleton::get_network_manager_instance();
    QUrl  url(ClientSingleton::get_http_urls_instance().get_download_file_url(), QUrl::StrictMode);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString     folder_path        = this->path_helper.get_workspace_path();
    QString     download_file_path = QString("%1/%2").arg(folder_path, file_name);
    QJsonObject json_data;
    json_data["file_path"] = download_file_path;
    QJsonDocument  json_doc(json_data);
    QByteArray     json_bytes = json_doc.toJson(QJsonDocument::Compact);
    QNetworkReply* reply      = manager.post(request, json_bytes);
    return reply;
}
// the urlencode will replace the '+' -> ' ',fuck!
void RemoteWorkspacePage::display_pdf_from_buffer_impl(const QString& file_name) {
    auto reply = this->send_download_file_req(file_name);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            reply->deleteLater();
            this->show_message("文件下载失败😫😫😫...");
            return;
        }
        auto data_bytes = reply->readAll();
        reply->deleteLater();
        QBuffer buffer(&data_bytes);
        buffer.open(QIODevice::ReadOnly);
        // this is important,must seek to start!
        buffer.seek(0);
        this->show_message("下载完成,正在渲染远程PDF文件😊😊😊", false);
        this->pdf_displayer->load_pdf(&buffer);
        show_and_raise(this->pdf_displayer);
    });

    connect(reply, &QNetworkReply::downloadProgress, this, [this](qint64 s1, qint64 s2) {
        this->ui->download_file_progress_bar->setRange(0, s2);
        this->ui->download_file_progress_bar->setValue(s1);
        // qDebug() << "Call download progress func in " << QThread::currentThreadId();
    });
}

void RemoteWorkspacePage::display_pdf_from_file_impl(const QString& file_name) {
    this->show_message("保存缓存文件...😂😂😂");
    auto&   cache_dir      = ClientSingleton::get_cache_file_dir();
    QString save_file_path = QString("%1/%2").arg(cache_dir, file_name);
    QString folder_path    = this->path_helper.get_workspace_path();
    QString src_file_path  = QString("%1/%2").arg(folder_path, file_name);
    emit    start_download_large_file(src_file_path, save_file_path);
}

void RemoteWorkspacePage::display_pdf_impl(const QString& file_name, size_t file_size) {
    constexpr size_t max_buffer_size = 128 * 1024 * 1024;   // 128M
    if (file_size <= max_buffer_size) {
        this->display_pdf_from_buffer_impl(file_name);
    } else {
        this->display_pdf_from_file_impl(file_name);
    }
}

void RemoteWorkspacePage::upload_file_impl() {
    QStringList file_names = QFileDialog::getOpenFileNames(this, "选择上传文件", "", "");
    if (file_names.empty()) {
        this->show_message("请选择文件🌷🌷🌷!");
        return;
    }
    QString save_dir = this->path_helper.get_workspace_path();
    emit    start_upload_files(file_names, save_dir);
}


void RemoteWorkspacePage::create_new_dir_impl(const QString& dir_name) {
    QJsonObject json_data;
    auto        dir_path = QString("%1/%2").arg(this->path_helper.get_workspace_path(), dir_name);
    ;
    json_data["file_path"] = dir_path;
    auto reply             = send_http_req_with_json_data(
        json_data, ClientSingleton::get_http_urls_instance().get_create_new_dir_url());
    connect(reply, &QNetworkReply::finished, this, [this, dir_path, reply]() {
        auto document = get_json_document(reply);

        if (!document) {
            this->show_message("网络请求错误😫😫😫");
            return;
        }
        auto json_data = document->object();

        int status = json_data[PublicResponseJsonKeys::status_key].toInt();
        if (status != static_cast<int>(StatusCode::kSuccess)) {
            this->show_message(json_data[PublicResponseJsonKeys::message_key].toString());
            return;
        }
        this->show_message(QString("成功创建了文件夹%1").arg(dir_path));
        // then flush the workspace content!
        this->get_workspace_content_impl(true);
    });
}

void RemoteWorkspacePage::display_right_menu_impl(const QModelIndex& index,
                                                  const QPoint&      global_pos) {
    // add the action impl here!,if need the position value!
    connect(right_menu->delete_action, &QAction::triggered, this, [this, index]() {
        auto& file_info = file_info_table_model->get_file_info(index.row());
        this->delete_file_dialog->hint_text->setText(
            QString("~~~是否要删除文件%1: '%2' ?\n~~~(请谨慎操作!!!😮😮😮)")
                .arg(file_info.file_type == FileKind::kFolder ? "夹" : "")
                .arg(file_info.file_name));
        this->delete_file_dialog->set_delete_filename(file_info.file_name);
        this->delete_file_dialog->display();
    });

    connect(right_menu->rename_action, &QAction::triggered, this, [this, index]() {
        auto& file_info = file_info_table_model->get_file_info(index.row());
        this->rename_file_dialog->previous_filename_value->setText(file_info.file_name);
        // QAQ
        this->rename_file_dialog->set_file_index(index.row());
        this->rename_file_dialog->display();
    });

    // return the choosed action!
    QAction* selected = right_menu->menu->exec(global_pos);
    if (selected == nullptr) {
        qDebug() << "Not select any action!";
    }
}


void RemoteWorkspacePage::display_right_menu_for_table_view(const QPoint& pos) {
    QModelIndex index = ui->workspace_content_table_view->indexAt(pos);
    this->display_right_menu_impl(index, this->ui->workspace_content_table_view->mapToGlobal(pos));
}

void RemoteWorkspacePage::display_right_menu_for_list_view(const QPoint& pos) {
    QModelIndex index = ui->workspace_content_list_view->indexAt(pos);
    this->display_right_menu_impl(index, this->ui->workspace_content_list_view->mapToGlobal(pos));
}


void RemoteWorkspacePage::delete_file_impl(const QString& filename) {
    QJsonObject json_data;
    auto        file_path = QString("%1/%2").arg(this->path_helper.get_workspace_path(), filename);
    ;
    json_data["file_path"] = file_path;
    auto reply             = send_http_req_with_json_data(
        json_data, ClientSingleton::get_http_urls_instance().get_delete_file_url());
    connect(reply, &QNetworkReply::finished, this, [this, file_path, reply]() {
        auto document = get_json_document(reply);
        if (!document) {
            this->show_message("网络请求错误😫😫😫");
            return;
        }
        auto json_data = document->object();
        int  status    = json_data[PublicResponseJsonKeys::status_key].toInt();
        if (status != static_cast<int>(StatusCode::kSuccess)) {
            this->show_message(json_data[PublicResponseJsonKeys::message_key].toString());
            return;
        }
        // this->refresh_workspace_content_impl();
        auto file_infos = ClientSingleton::get_cache_workspace_data_instance().get_file_infos(
            path_helper.get_workspace_path());
        auto index = this->delete_file_dialog->get_delete_file_index();
        if (file_infos.empty() || index >= file_infos.size()) {
            this->show_message("发生了错误,文件信息获取失败😫😫😫");
            return;
        }
        // earas is delete by value...
        std::copy(file_infos.data() + index + 1,
                  file_infos.data() + file_infos.size(),
                  file_infos.data() + index);
        this->show_message(QString("成功删除了文件%1 😊😊😊").arg(file_path), false);
    });
}

void RemoteWorkspacePage::rename_file_impl(const QString& src_filename,
                                           const QString& dst_filename) {
    // prepare data
    QJsonObject json_data;
    QString     folder_path       = path_helper.get_workspace_path();
    auto        src               = QString("%1/%2").arg(folder_path, src_filename);
    auto        dst               = QString("%1/%2").arg(folder_path, dst_filename);
    json_data["source_file_path"] = src;
    json_data["target_file_path"] = dst;
    // qDebug() << json_data;
    auto reply = send_http_req_with_json_data(
        json_data, ClientSingleton::get_http_urls_instance().get_rename_file_url());
    connect(reply, &QNetworkReply::finished, this, [this, src_filename, dst_filename, reply]() {
        auto document = get_json_document(reply);
        if (!document) {
            this->show_message("网络请求错误😫😫😫");
            return;
        }
        auto json_data = document->object();
        int  status    = json_data[PublicResponseJsonKeys::status_key].toInt();
        if (status != static_cast<int>(StatusCode::kSuccess)) {
            this->show_message(json_data[PublicResponseJsonKeys::message_key].toString());
            return;
        }
        // this->refresh_workspace_content_impl();
        // assign the value is better! is better!
        auto file_infos = ClientSingleton::get_cache_workspace_data_instance().get_file_infos(
            path_helper.get_workspace_path());
        auto index = this->rename_file_dialog->get_file_index();
        if (file_infos.empty() || this->rename_file_dialog->get_file_index() >= file_infos.size()) {
            this->show_message("发生了错误,文件信息获取失败😫😫😫");
            return;
        }
        file_infos[index].file_name = dst_filename;
        this->show_message(
            QString("修改文件名 %1 -> %2 😊😊😊").arg(src_filename).arg(dst_filename));
    });
}


}   // namespace client
}   // namespace tang