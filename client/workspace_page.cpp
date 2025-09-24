#include "workspace_page.h"
#include "ElaMessageBar.h"
#include "client_global_config.h"
#include "client_singleton.h"
#include "common/response_keys.h"
#include "util.h"
#include "workspace_page_ui.h"
#include "workspace_view_model.h"
#include <QFont>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSplitter>
#include <QStackedWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QUrlQuery>
#include <fstream>
#include <QVBoxLayout>
#include <QBuffer>

using namespace tang::common;
namespace tang {
namespace client {
struct WorkspaceResJsonValidator {
    static bool validate(const QJsonObject& json_data) {
        constexpr auto keys = WorkspaceResJsonKeys::keys;
        for (size_t i = 0; i < keys.size(); ++i) {
            if (!json_data.contains(keys[i])) {
                return false;
            }
            // validate type!
            if (!json_data[keys[i]].isArray()) {
                return false;
            }
            auto values = json_data[keys[i]].toArray();
            for (auto value : values) {
                if (!value.isString()) {
                    break;
                }
            }
        }

        return true;
    }
};

struct WorkspaceContentResJsonValidator {
    static bool validate(const QJsonObject& json_data) {
        if (!json_data.contains(WorkspaceContentResponse::file_infos_key)) {
            return false;
        }

        auto file_infos_json = json_data[WorkspaceContentResponse::file_infos_key];
        if (!file_infos_json.isArray()) {
            return false;
        }
        auto file_infos = file_infos_json.toArray();
        for (size_t i = 0; i < file_infos.size(); ++i) {
            auto item = file_infos[i];
            if (!item.isObject()) {
                return false;
            }
            // then validate keys
            constexpr auto keys = WorkspaceContentResponse::item_keys;
            for (size_t j = 0; j < keys.size(); ++j) {
                if (!item.toObject().contains(keys[j])) {
                    return false;
                }
            }
            // then validate the type!
            if (!item.toObject()[WorkspaceContentResponse::file_name_key].isString() ||
                !item.toObject()[WorkspaceContentResponse::file_size_key].isDouble() ||
                !item.toObject()[WorkspaceContentResponse::file_type_key].isDouble() ||
                !item.toObject()[WorkspaceContentResponse::last_write_time_key].isString()) {
                return false;
            }
        }

        return true;
    }
};
// jsut for test!!!
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
    , display_pdf(new DisplayPdf())
    , show_widget(find_root_widget(this)) {
    // reverse capacity
    ui->setup_ui(this);
    this->setTitleVisible(false);
    // this is not enabled!
    // this->setWindowTitle("workspace");
    // only for test
    add_test();
    // set models
    workspace_model = new RemoteWorkspaceInfoModel({}, ui->workspace_content_list_view);
    workspace_model->set_workspace_names(
        ClientSingleton::get_cache_workspace_data_instance().get_workspace_show_names());
    qDebug()
        << "workspace count:"
        << ClientSingleton::get_cache_workspace_data_instance().get_workspace_show_names().size();
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
}

RemoteWorkspacePage::~RemoteWorkspacePage() {
    delete ui;
    delete display_pdf;
}

void RemoteWorkspacePage::show_message(const QString& message, bool error) {
    if (error) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "workspace",
                               message,
                               ClientGlobalConfig::error_show_time,
                               show_widget);
    } else {
        ElaMessageBar::success(ElaMessageBarType::TopRight,
                               "workspace",
                               message,
                               ClientGlobalConfig::message_show_time,
                               show_widget);
    }
}
void RemoteWorkspacePage::initialize_connects() {
    connect(ui->workspace_content_table_view, &ElaTableView::tableViewShow, this, [this]() {
        size_t width = this->ui->workspace_content_table_view->width();
        size_t w0    = static_cast<size_t>(width * 0.1);
        size_t w1    = static_cast<size_t>(width * 0.35);
        size_t w2    = static_cast<size_t>(width * 0.15);
        size_t w3    = static_cast<size_t>(width * 0.15);
        size_t w4    = static_cast<size_t>(width * 0.25);
        ui->workspace_content_table_view->setColumnWidth(0, w0);
        ui->workspace_content_table_view->setColumnWidth(1, w1);
        ui->workspace_content_table_view->setColumnWidth(2, w2);
        ui->workspace_content_table_view->setColumnWidth(3, w3);
        ui->workspace_content_table_view->setColumnWidth(4, w4);
    });


    connect(ui->workspace_view,
            &ElaListView::clicked,
            this,
            &RemoteWorkspacePage::on_workspace_item_clicked);

    connect(ui->workspace_content_table_view,
            &ElaTableView::doubleClicked,
            this,
            &RemoteWorkspacePage::on_workspace_table_content_item_clicked);

    connect(ui->workspace_content_list_view,
            &ElaListView::doubleClicked,
            this,
            &RemoteWorkspacePage::on_workspace_list_content_item_clicked);

    connect(ui->flush_workspace_name_button,
            &ElaToolButton::clicked,
            this,
            &RemoteWorkspacePage::on_flush_workspace_name_button_clicked);

    connect(ui->flush_workspace_content_button,
            &ElaToolButton::clicked,
            this,
            &RemoteWorkspacePage::on_flush_workspace_content_button_clicked);
    connect(ui->view_tiling_button,
            &ElaToolButton::clicked,
            this,
            &RemoteWorkspacePage::on_view_tiling_button_clicked);
    connect(ui->view_detail_button,
            &ElaToolButton::clicked,
            this,
            &RemoteWorkspacePage::on_view_detail_button_clicked);

    connect(ui->back_button,
            &ElaToolButton::clicked,
            this,
            &RemoteWorkspacePage::on_back_button_clicked);
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

void RemoteWorkspacePage::on_flush_workspace_content_button_clicked() {
    QString workspace_path = ui->directory_line_edit->text();
    if (workspace_path.isEmpty()) {
        show_message("workspace为空🤣🤣🤣...");
        return;
    }
    this->get_workspace_content_impl(true);
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
        show_message("从缓存获取(you can refresh) 😊😊😊");
        ui->directory_line_edit->setText(folder_path);
        this->set_workspace_content_data(file_infos);
    }
}

void RemoteWorkspacePage::send_get_workspace_content_req(const QString& folder_path) {
    QNetworkRequest request(ClientSingleton::get_http_urls_instance().get_workspace_content_url());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    // no need any params!
    auto&     manager = ClientSingleton::get_network_manager_instance();
    QUrlQuery query;
    // auto [l, r]           = remove_path_sep(folder_path);
    // if (!(l == 0 && r == folder_path.size() - 1)) {
    //     folder_path = folder_path.mid(l, r - l + 1);
    //     // then set to ui
    //     ui->directory_line_edit->setText(folder_path);
    // }
    query.addQueryItem("folder_path", folder_path);
    QByteArray     query_data = query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkReply* reply      = manager.post(request, query_data);
    show_message("正在查询服务器😴😴😴...", false);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        if (this->process_workspace_content_response(reply)) {
            this->ui->directory_line_edit->setText(this->path_helper.get_workspace_show_path());
        } else {
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


void RemoteWorkspacePage::on_workspace_table_content_item_clicked(const QModelIndex& index) {
    if (!index.isValid()) {
        return;
    }
    int row = index.row();
    int col = index.column();

    // if folder
    auto& file_info = file_info_table_model->get_file_info(row);
    if (file_info.file_type == FileKind::kFolder) {
        this->enter_folder_impl(file_info.file_name);
    }else if(file_info.file_type == FileKind::kPdf){
        //try to open it!
        QString file_path = "C:/Users/10995/Documents/learning/048Qt+6+C++开发指南.pdf";
        std::ifstream reader(file_path.toStdWString());
        reader.seekg(0,std::ios_base::end);
        auto size = reader.tellg();
        QByteArray datas;
        show_message(QString("file size %1").arg(size),false);
        datas.resize(size);
        reader.seekg(0,std::ios_base::beg);
        reader.read(datas.data(),size);
        QBuffer buffer(this);
        buffer.setBuffer(&datas);
        display_pdf->show();
        display_pdf->load_pdf(file_path);
        
    }else{
        
    }
}

void RemoteWorkspacePage::on_workspace_list_content_item_clicked(const QModelIndex& index) {
    if (!index.isValid()) {
        return;
    }
    int   row       = index.row();
    auto& file_info = file_info_list_model->get_file_info(row);
    if (file_info.file_type == FileKind::kFolder) {
        this->enter_folder_impl(file_info.file_name);
    }
}


void RemoteWorkspacePage::on_back_button_clicked() {
    this->path_helper.pop_back_safe();
    this->get_workspace_content_impl(false);
}
}   // namespace client
}   // namespace tang