#pragma once

#include "ElaScrollPage.h"
#include "workspace_page_ui.h"
#include <QStackedWidget>

// we should save the model!
#include "display_pdf_file.h"
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
                return QString("%1").arg(workspace);
            }
            // attention,do not add this!
            // our path is relative
            return QString("%1/%2").arg(workspace).arg(workspace_items.join('/'));
        }

        void append(const QString& item) { workspace_items.push_back(item); }

        void append(QString&& item) { workspace_items.push_back(std::move(item)); }

        void clear() {
            workspace.clear();
            workspace_show_name.clear();
            workspace_items.clear();
        }

        size_t item_size() { return workspace_items.size(); }

        void pop_back_safe() {
            if (workspace_items.size() > 0) {
                workspace_items.pop_back();
            }
        }
    };
    RemoteWorkspacePageUi* ui;
    // support the datas
    RemoteWorkspaceInfoModel*    workspace_model;
    RemoteFileInfoViewModel*     file_info_table_model;
    RemoteFileInfoListViewModel* file_info_list_model;
    DisplayPdf*                  pdf_displayer;
    WorkspacePathHelper          path_helper;

    // the widget to show message with universal style!
    QWidget* show_widget;
    // process workspace names
    void send_get_workspace_req();
    void process_workspace_response(QNetworkReply* reply);

    // process workspace content
    void get_workspace_content_impl(bool refresh);
    void send_get_workspace_content_req(const QString& workspace_path);
    bool process_workspace_content_response(QNetworkReply* reply);

    void show_message(const QString& message, bool error = true);

    void enter_folder_impl(const QString& folder_name);

    void set_workspace_content_data(std::span<RemoteFileInfo> file_infos);

    void set_workspace_data(std::span<QString> workspaces);

    // display pdf file!
    void display_pdf_from_buffer_impl(const QString& file_name);
    // void display_pdf_from_file_impl(const QString& file_name);
    // void display_pdf(QString& file_name,size_t file_size);

    // adjust table content view!
    void adjust_workspace_content_view();


public:
    RemoteWorkspacePage(QWidget* parent = nullptr);
    ~RemoteWorkspacePage();
    void initialize_connects();


public slots:
    // workspace
    void on_workspace_item_clicked(const QModelIndex& index);
    // workspace content!
    void on_workspace_table_content_item_clicked(const QModelIndex& index);
    void on_workspace_list_content_item_clicked(const QModelIndex& index);

    void on_flush_workspace_name_button_clicked();
    void on_flush_workspace_content_button_clicked();
    void on_view_tiling_button_clicked();
    void on_view_detail_button_clicked();
    void on_back_button_clicked();

    void on_adjust_content_view_button_clicked();

    // void on_forward_button_clicked();
};

}   // namespace client
}   // namespace tang