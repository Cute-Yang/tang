#pragma once

#include "ElaScrollPage.h"
#include "workspace_page_ui.h"
#include <QStackedWidget>

// we should save the model!
#include "workspace_view_model.h"


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

public:
    RemoteWorkspacePage(QWidget* parent = nullptr);
    ~RemoteWorkspacePage();
    void initialize_connects();

public slots:
    void click_workspace_item(const QModelIndex& index);

    void click_workspace_list_item(const QModelIndex& index);
    
};

}   // namespace client
}   // namespace tang