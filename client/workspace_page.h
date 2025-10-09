#pragma once

#include "ElaScrollPage.h"
#include "workspace_page_ui.h"
#include <QStackedWidget>
// we should save the model!
#include "display_pdf_file.h"
#include "long_time_http_task.h"
#include "new_dir.h"
#include "workspace_delete.h"
#include "workspace_rename_file.h"
#include "workspace_right_menu.h"
#include "workspace_view_model.h"
#include <QFile>
#include <QJsonObject>
#include <QNetworkReply>
#include <QThread>
#include "workspace_helper.h"

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
    DisplayPdf*                  pdf_displayer;
    WorkspacePathHelper          path_helper;

    // the widget to show message with universal style!
    QWidget*          show_widget;
    QThread*          long_time_http_runner;
    LongtimeHttpTask* long_time_http_worker;

    NewDir*              new_dir_dialog;
    WorkspaceRightMenu*  right_menu;
    WorkspaceRenameFile* rename_file_dialog;
    WorkspaceDeleteFile* delete_file_dialog;


    // some helper functions...
    //  process workspace names
    void send_get_workspace_req();
    void process_workspace_response(QNetworkReply* reply);

    // process workspace content
    void           get_workspace_content_impl(bool refresh);
    void           send_get_workspace_content_req(const QString&          workspace_path,
                                                  std::function<void()>&& success_callback = nullptr,
                                                  std::function<void()>&& failed_callback = nullptr);
    bool           process_workspace_content_response(QNetworkReply* reply);
    void           show_message(const QString& message, bool error = true);
    void           enter_folder_impl(const QString& folder_name);
    void           set_workspace_content_data(std::span<RemoteFileInfo> file_infos);
    void           set_workspace_data(std::span<QString> workspaces);
    QNetworkReply* send_download_file_req(const QString& file_name);

    // display pdf file!
    void display_pdf_from_buffer_impl(const QString& file_name);
    void display_pdf_from_file_impl(const QString& file_name);
    void display_pdf_impl(const QString& file_name, size_t file_size);

    // adjust table content view!
    void adjust_workspace_content_view();
    void initialize_connects();
    void create_new_dir_impl(const QString& dir_name);

    void set_workspace_content_icon_size_impl(QSize icon_size);

    void delete_file_impl(QString const& file_name);
    bool delete_cache_file_info_impl(size_t index);

    

    void rename_file_impl(QString const& src_filename, QString const& dst_filename);
    void refresh_workspace_content_impl();

    void workspace_content_double_clicked_impl(const RemoteFileInfo& file_info);

    void refresh_workspace_name_impl();

    void upload_file_impl();

    void change_workspace_impl(size_t i);

public:
    RemoteWorkspacePage(QWidget* parent = nullptr);
    ~RemoteWorkspacePage();
    void refresh_for_once();

public slots:
    // workspace
    void on_workspace_item_clicked(const QModelIndex& index);
    void on_view_tiling_button_clicked();
    void on_view_detail_button_clicked();
    void on_back_button_clicked();

    void on_adjust_content_view_button_clicked();

    void display_right_menu_for_table_view(const QPoint& pos);
    void display_right_menu_for_list_view(const QPoint& pos);
    void display_right_menu_impl(const QModelIndex& index, const QPoint& global_pos);


    // void on_forward_button_clicked();
signals:
    void start_download_large_file(const QString& src_file_path, const QString& save_file_path);
    void start_upload_files(const QStringList& upload_files, const QString& save_dir);
    void start_rename_file(int file_index);
    void start_delete_file(int file_index);
};

}   // namespace client
}   // namespace tang