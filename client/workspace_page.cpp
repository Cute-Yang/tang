#include "workspace_page.h"
#include "ElaMessageBar.h"
#include "client_singleton.h"
#include "util.h"
#include "workspace_page_ui.h"
#include "workspace_view_model.h"
#include <QFont>
#include <QHeaderView>
#include <QSplitter>
#include <QStackedWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QVBoxLayout>


using namespace tang::common;
namespace tang {
namespace client {

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
    workspace_data_cache.set_workspace_names(std::move(test_workspaces));
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
        ClientSingleton::get_cache_workspace_data_instance().get_workspace_names());
    qDebug() << "workspace count:"
             << ClientSingleton::get_cache_workspace_data_instance().get_workspace_names().size(); 
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
    qDebug() << "haha";
    if (!index.isValid()) {
        return;
    }
    int     row     = index.row();
    QString message = QString("切换工作空间为 %1").arg(row);

    ElaMessageBar::information(ElaMessageBarType::TopRight, "switch", message, 2700, this);
}

}   // namespace client
}   // namespace tang