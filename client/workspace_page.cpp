#include "workspace_page.h"
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
#include "ElaMessageBar.h"

namespace tang {
namespace client {

RemoteWorkspacePage::RemoteWorkspacePage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new RemoteWorkspacePageUi()) {
    // this is not enabled!
    // this->setWindowTitle("workspace");
    ui->setup_ui(this);
    this->setTitleVisible(false);
    this->set_test_model();
    // set the column width
    connect(ui->workspace_content_table_view, &ElaTableView::tableViewShow, this, [this]() {
        ui->workspace_content_table_view->setColumnWidth(0, 36);
        ui->workspace_content_table_view->setColumnWidth(1, 150);
        ui->workspace_content_table_view->setColumnWidth(2, 40);
        ui->workspace_content_table_view->setColumnWidth(3, 80);
        ui->workspace_content_table_view->setColumnWidth(4, 160);
    });

    connect(ui->workspace_content_table_view,&ElaTableView::clicked,this,&RemoteWorkspacePage::click_workspace_item);
    connect(ui->workspace_view,&ElaListView::clicked,this,&RemoteWorkspacePage::click_workspace_list_item);
}


// jsut for test!!!
void RemoteWorkspacePage::set_test_model() {
    QIcon          folder_icon(":icons/images/workspace.svg");
    QList<QString> test_workspaces = {"唐远志", "lazydog", "Arics", "jack", "Moris", "Moon"};

    RemoteWorkspaceInfoModel* workspace_model =
        new RemoteWorkspaceInfoModel(test_workspaces, this->ui->workspace_content_list_view);


    this->ui->workspace_view->setModel(workspace_model);

    QStringList headers = { "名称", "类型", "大小", "修改时间"};

    QList<RemoteFileInfo> file_infos_data = {
        {"images", 4096, SimpleFileType::k_foler, "2023-04-05 12:32:51"},
        {"report.docx", 1024, SimpleFileType::k_word, "2023-04-05 12:32:51"},

        {"simulation.py", 1024, SimpleFileType::k_python, "2021-05-05 13:32:51"},

        {"mat.cpp", 1024, SimpleFileType::k_cpp, "2023-04-05 12:32:51"},
        {"sunny", 1024, SimpleFileType::k_foler, "2023-04-05 12:32:51"},
        {"show.ppt", 1024, SimpleFileType::k_ppt, "2023-04-05 12:32:51"},
        {"pay_for_month_9.pdf", 1024, SimpleFileType::k_pdf, "2023-04-05 12:32:51"},

        {"simulation.xls", 1024, SimpleFileType::k_excel, "2021-05-05 13:32:51"},

        {"mat_gemm.cpp", 21305, SimpleFileType::k_cpp, "2023-04-05 12:32:51"},
        {"pipeline.txt", 1024, SimpleFileType::k_txt, "2023-04-05 12:32:51"},
        {"works", 1024, SimpleFileType::k_foler, "2023-04-05 12:32:51"},
        {"guassian_blur.cpp", 1024, SimpleFileType::k_cpp, "2023-04-05 12:32:51"},

        {"test_gradient.py", 1024, SimpleFileType::k_python, "2021-05-05 13:32:51"},

        {"canny_test.cpp", 1024, SimpleFileType::k_cpp, "2023-04-05 12:32:51"},
        {"fish", 1024, SimpleFileType::k_foler, "2023-04-05 12:32:51"},
        {"data_process", 1024, SimpleFileType::k_foler, "2023-04-05 12:32:51"},
        {"transform.cpp", 1024, SimpleFileType::k_cpp, "2023-04-05 12:32:51"},

        {"merge_result.xls", 1024, SimpleFileType::k_excel, "2021-05-05 13:32:51"}

    };
    std::shared_ptr<QList<RemoteFileInfo>> file_infos =
        std::make_shared<QList<RemoteFileInfo>>(std::move(file_infos_data));

    RemoteFileInfoViewModel* file_info_model = new RemoteFileInfoViewModel(headers, file_infos);
    this->ui->workspace_content_table_view->setModel(file_info_model);

    RemoteFileInfoListViewModel* file_info_list_model = new RemoteFileInfoListViewModel(file_infos);
    this->ui->workspace_content_list_view->setModel(file_info_list_model);
}

void RemoteWorkspacePage::click_workspace_item(const QModelIndex& index){
    if(!index.isValid()){
        return;
    }

    int row = index.row();
    int col = index.column();
    qDebug() << "row:" << row << " col:" << col;

    //here need to give format arg position
    QString message = QString("click at row = %1 col = %2").arg(row).arg(col);

    ElaMessageBar::information(ElaMessageBarType::TopRight,"点击workspace",message,2700,this);

}

void RemoteWorkspacePage::click_workspace_list_item(const QModelIndex& index){
    qDebug() << "haha";
    if(!index.isValid()){
        return;
    }
    int row = index.row();
    QString message = QString("切换工作空间为 %1").arg(row);
    
    ElaMessageBar::information(ElaMessageBarType::TopRight,"switch",message,2700,this);

    
}

}   // namespace client
}   // namespace tang