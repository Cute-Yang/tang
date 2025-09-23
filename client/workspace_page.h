#pragma once

#include "ElaScrollPage.h"
#include "workspace_page_ui.h"
#include <QStackedWidget>

// we should save the model!
#include "workspace_view_model.h"
#include <QNetworkReply>


namespace tang {
namespace client {
class RemoteWorkspacePage : public ElaScrollPage {
    Q_OBJECT
private:
    RemoteWorkspacePageUi* ui;
    // support the datas
    RemoteWorkspaceInfoModel*    workspace_model;
    RemoteFileInfoViewModel*     file_info_table_model;
    RemoteFileInfoListViewModel* file_info_list_model;

    // process workspace names
    void send_get_workspace_req();
    void process_workspace_response(QNetworkReply* reply);

    // process workspace content
    void send_get_workspace_content_req();
    void process_workspace_content_response(QNetworkReply* reply);

public:
    RemoteWorkspacePage(QWidget* parent = nullptr);
    ~RemoteWorkspacePage();
    void initialize_connects();

public slots:
    void click_workspace_item(const QModelIndex& index);

    void click_workspace_list_item(const QModelIndex& index);

    void on_flush_workspace_name_button_clicked();

    void on_flush_workspace_content_button_clicked();
};

}   // namespace client
}   // namespace tang