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
    struct WorkspacePathHelper {
        QStringList workspace_items;
        // the show name!
        QString workspace_show_name;
        QString workspace;
        WorkspacePathHelper() { workspace_items.reserve(8); }

        void set_workspace(const QString& workspace_) { workspace = workspace_; }
        void set_workspace_show_name(const QString& workspace_show_name_) {
            workspace_show_name = workspace_show_name_;
        }
        QString get_workspace_show_path() {
            if (workspace_items.size() == 0) {
                return QString("/%1").arg(workspace_show_name);
            }
            return QString("/%1/%2").arg(workspace_show_name).arg(workspace_items.join('/'));
        }
        QString get_workspace_path() {
            if (workspace_items.size() == 0) {
                return QString("/%1").arg(workspace);
            }
            return QString("/%1/%2").arg(workspace).arg(workspace_items.join('/'));
        }

        void append(const QString& item) { workspace_items.push_back(item); }

        void append(QString&& item) { workspace_items.push_back(std::move(item)); }

        void clear() {
            workspace.clear();
            workspace_show_name.clear();
            workspace_items.clear();
        }
    };
    RemoteWorkspacePageUi* ui;
    // support the datas
    RemoteWorkspaceInfoModel*    workspace_model;
    RemoteFileInfoViewModel*     file_info_table_model;
    RemoteFileInfoListViewModel* file_info_list_model;
    WorkspacePathHelper          path_helper;

    // the widget to show message with universal style!
    QWidget* show_widget;
    // process workspace names
    void send_get_workspace_req();
    void process_workspace_response(QNetworkReply* reply);

    // process workspace content
    void send_get_workspace_content_req();
    void process_workspace_content_response(QNetworkReply* reply);

    void show_message(const QString& message, bool error = true);

public:
    RemoteWorkspacePage(QWidget* parent = nullptr);
    ~RemoteWorkspacePage();
    void initialize_connects();

public slots:
    void click_workspace_item(const QModelIndex& index);
    // row!
    void click_workspace_table_content_item(const QModelIndex& index);
    // row and col!
    // void click_workspace_list_content_item(const QModelIndex& index);

    void on_flush_workspace_name_button_clicked();
    void on_flush_workspace_content_button_clicked();
    void on_view_tiling_button_clicked();
    void on_view_detail_button_clicked();
};

}   // namespace client
}   // namespace tang