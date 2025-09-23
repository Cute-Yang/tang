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
#include <QVBoxLayout>


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

// jsut for test!!!
void add_test() {
    std::vector<QString> test_workspaces = {"唐远志", "lazydog", "Arics", "jack", "Moris", "Moon"};
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

    // if you reset the data of cache worspace,remember to set the workspace names!because they are
    // views!
    auto& workspace_data_cache = ClientSingleton::get_cache_workspace_data_instance();
    workspace_data_cache.set_file_infos("唐远志", std::move(file_infos_data));
    workspace_data_cache.set_workspace_show_names(std::move(test_workspaces));
}

RemoteWorkspacePage::RemoteWorkspacePage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new RemoteWorkspacePageUi()) {
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

    QStringList headers = {"名称", "类型", "大小", "修改时间"};
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
}

void RemoteWorkspacePage::initialize_connects() {
    connect(ui->workspace_content_table_view, &ElaTableView::tableViewShow, this, [this]() {
        ui->workspace_content_table_view->setColumnWidth(0, 36);
        ui->workspace_content_table_view->setColumnWidth(1, 150);
        ui->workspace_content_table_view->setColumnWidth(2, 40);
        ui->workspace_content_table_view->setColumnWidth(3, 80);
        ui->workspace_content_table_view->setColumnWidth(4, 160);
    });

    connect(ui->workspace_content_table_view,
            &ElaTableView::clicked,
            this,
            &RemoteWorkspacePage::click_workspace_item);
    connect(ui->workspace_view,
            &ElaListView::clicked,
            this,
            &RemoteWorkspacePage::click_workspace_list_item);

    connect(ui->flush_workspace_name_button,
            &ElaToolButton::clicked,
            this,
            &RemoteWorkspacePage::on_flush_workspace_name_button_clicked);
}



void RemoteWorkspacePage::click_workspace_item(const QModelIndex& index) {
    if (!index.isValid()) {
        return;
    }

    int row = index.row();
    int col = index.column();
    qDebug() << "row:" << row << " col:" << col;

    // here need to give format arg position
    QString message = QString("click at row = %1 col = %2").arg(row).arg(col);

    ElaMessageBar::information(ElaMessageBarType::TopRight, "点击workspace", message, 2700, this);
}

void RemoteWorkspacePage::click_workspace_list_item(const QModelIndex& index) {
    if (!index.isValid()) {
        return;
    }
    int     row     = index.row();
    QString message = QString("切换工作空间为 %1").arg(row);
    ElaMessageBar::information(ElaMessageBarType::TopRight, "switch", message, 2700, this);
}


void RemoteWorkspacePage::on_flush_workspace_name_button_clicked() {
    if (!ui->show_all_workspaces->getIsToggled()) {
        ElaMessageBar::information(ElaMessageBarType::TopRight,
                                   "flush",
                                   "仅显示自己的工作区",
                                   ClientGlobalConfig::message_show_time,
                                   this);
        auto& workspace_data_cache = ClientSingleton::get_cache_workspace_data_instance();
        workspace_data_cache.set_workspaces({public_workspace_name.data(), "9"});
        workspace_data_cache.set_workspace_show_names({public_workspace_name.data(), "周琳"});
        workspace_model->set_workspace_names(workspace_data_cache.get_workspace_show_names());
        workspace_model->layoutChanged();
        // after login
        return;
    }
    this->send_get_workspace_req();

    // just for test!
    // auto& workspace_data_cache           = ClientSingleton::get_cache_workspace_data_instance();
    // std::vector<QString> test_workspaces = {
    //     "犬夜叉", "戈薇", "桔梗", "弥勒", "珊瑚", "七宝", "奈落", "琥珀", "翡翠"};
    // workspace_data_cache.set_workspace_show_names(std::move(test_workspaces));
    // workspace_model->set_workspace_names(workspace_data_cache.get_workspace_show_names());
    // workspace_model->layoutChanged();
};


void RemoteWorkspacePage::process_workspace_response(QNetworkReply* reply) {
    auto show_widget = find_root_widget(this);
    auto document    = get_json_document(reply);
    if (!document) {
        ElaMessageBar::error(ElaMessageBarType::TopRight,
                             "flush",
                             "网络异常!",
                             ClientGlobalConfig::message_show_time,
                             show_widget);
        return;
    }
    auto&       document_value = document.value();
    QJsonObject json_data      = document_value.object();
    if (!WorkspaceResJsonValidator::validate(json_data)) {
        ElaMessageBar::error(ElaMessageBarType::TopRight,
                             "flush",
                             "返回格式错误!",
                             ClientGlobalConfig::message_show_time,
                             show_widget);
        return;
    }

    if (json_data[PublicResponseJsonKeys::status_key].toInt() !=
        static_cast<int>(StatusCode::kSuccess)) {
        ElaMessageBar::error(ElaMessageBarType::TopRight,
                             "flush",
                             QString("获取失败 (reason:%1)")
                                 .arg(json_data[PublicResponseJsonKeys::message_key].toString()),
                             ClientGlobalConfig::message_show_time,
                             show_widget);
        return;
    }

    auto& cache_workspace_data = ClientSingleton::get_cache_workspace_data_instance();
    auto  workspaces_json      = json_data[WorkspaceResJsonKeys::workspaces_key].toArray();

    auto workspace_show_names_json =
        json_data[WorkspaceResJsonKeys::workspace_show_names_key].toArray();
    if (workspaces_json.size() != workspace_show_names_json.size()) {
        ElaMessageBar::error(ElaMessageBarType::TopRight,
                             "login",
                             "返回格式错误!",
                             ClientGlobalConfig::message_show_time,
                             show_widget);
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
    ElaMessageBar::success(ElaMessageBarType::TopRight,
                           "flush",
                           "成功获取所有工作区!",
                           ClientGlobalConfig::message_show_time,
                           this);
    workspace_model->set_workspace_names(cache_workspace_data.get_workspace_show_names());
    workspace_model->layoutChanged();
}

void RemoteWorkspacePage::send_get_workspace_req() {
    auto            show_widget = find_root_widget(this);
    QNetworkRequest request(ClientSingleton::get_http_urls_instance().get_workspace_url());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    // no need any params!
    auto&          manager = ClientSingleton::get_network_manager_instance();
    QNetworkReply* reply   = manager.get(request);
    ElaMessageBar::success(ElaMessageBarType::TopRight,
                           "flush",
                           "正在从服务器获取资源,请稍后...",
                           ClientGlobalConfig::message_show_time,
                           this);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        this->process_workspace_response(reply);
    });
}

void RemoteWorkspacePage::on_flush_workspace_content_button_clicked(){
    
}


}   // namespace client
}   // namespace tang