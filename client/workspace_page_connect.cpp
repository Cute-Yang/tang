#include "client_global_config.h"
#include "client_singleton.h"
#include "common/status.h"
#include "workspace_page.h"


using namespace tang::common;


namespace tang {
namespace client {
void RemoteWorkspacePage::initialize_connects() {
    // initialize all the connects
    connect(long_time_http_runner,
            &QThread::finished,
            long_time_http_worker,
            &LongtimeHttpTask::deleteLater);

    connect(ui->workspace_content_table_view,
            &ElaTableView::tableViewShow,
            this,
            &RemoteWorkspacePage::adjust_workspace_content_view);
    connect(ui->workspace_view,
            &ElaListView::clicked,
            this,
            &RemoteWorkspacePage::on_workspace_item_clicked);

    connect(ui->workspace_content_table_view,
            &ElaTableView::doubleClicked,
            this,
            [this](const QModelIndex& index) {
                if (!index.isValid()) {
                    return;
                }
                int row = index.row();
                int col = index.column();

                // if folder
                auto& file_info = file_info_table_model->get_file_info(row);
                this->workspace_content_double_clicked_impl(file_info);
            });

    connect(ui->workspace_content_list_view,
            &ElaListView::doubleClicked,
            this,
            [this](const QModelIndex& index) {
                if (!index.isValid()) {
                    return;
                }
                int row = index.row();

                // if folder
                auto& file_info = file_info_list_model->get_file_info(row);
                this->workspace_content_double_clicked_impl(file_info);
            });

    connect(ui->flush_workspace_name_button,
            &ElaToolButton::clicked,
            this,
            &RemoteWorkspacePage::on_flush_workspace_name_button_clicked);

    connect(ui->flush_workspace_content_button, &ElaToolButton::clicked, this, [this]() {
        this->refresh_workspace_content_impl();
    });

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

    connect(ui->adjust_content_view_button,
            &ElaToolButton::clicked,
            this,
            &RemoteWorkspacePage::on_adjust_content_view_button_clicked);

    connect(long_time_http_worker,
            &LongtimeHttpTask::finish_download_file,
            this,
            [this](bool success, const QString& message, const QString& save_path) {
                this->show_message(message, !success);
                qDebug() << message;
                if (success) {
                    this->pdf_displayer->load_pdf(save_path);
                    show_and_raise(this->pdf_displayer);
                }
            });
    connect(this,
            &RemoteWorkspacePage::start_download_large_file,
            long_time_http_worker,
            &LongtimeHttpTask::download_large_file);

    connect(long_time_http_worker,
            &LongtimeHttpTask::download_percent,
            this->ui->download_file_progress_bar,
            &ElaProgressBar::setValue);

    connect(
        ui->upload_file_action, &QAction::triggered, this, [this]() { this->upload_file_impl(); });

    connect(this,
            &RemoteWorkspacePage::start_upload_files,
            long_time_http_worker,
            &LongtimeHttpTask::upload_files);

    connect(long_time_http_worker,
            &LongtimeHttpTask::finish_upload_files,
            this,
            [this](int n1, int n2) {
                this->show_message(QString("成功上传:%1,失败:%2").arg(n1).arg(n2), n2 > 0);
            });
    connect(long_time_http_worker,
            &LongtimeHttpTask::upload_percent,
            this->ui->upload_file_progress_bar,
            &ElaProgressBar::setValue);

    connect(this->ui->new_dir_action, &QAction::triggered, this, [this]() {
        this->new_dir_dialog->display();
    });

    connect(new_dir_dialog->ui->ok_button, &ElaToolButton::clicked, this, [this]() {
        // first,flush the workspace content!
        auto new_dir_name = new_dir_dialog->ui->line_edit->text();
        if (new_dir_name.isEmpty()) {
            this->show_message("名称不能为空😮😮😮");
            return;
        }
        this->new_dir_dialog->clear();
        this->create_new_dir_impl(new_dir_name);
    });

    connect(new_dir_dialog->ui->cancle_button, &ElaToolButton::clicked, this, [this]() {
        this->new_dir_dialog->clear();
    });

    // view
    connect(ui->view_file_with_small_icon_action, &QAction::triggered, this, [this]() {
        this->set_workspace_content_icon_size_impl(
            QSize(ClientGlobalConfig::small_icon_size, ClientGlobalConfig::small_icon_size));
    });

    connect(ui->view_file_with_middle_icon_action, &QAction::triggered, this, [this]() {
        this->set_workspace_content_icon_size_impl(
            QSize(ClientGlobalConfig::middle_icon_size, ClientGlobalConfig::middle_icon_size));
    });

    connect(ui->view_file_with_large_icon_action, &QAction::triggered, this, [this]() {
        this->set_workspace_content_icon_size_impl(
            QSize(ClientGlobalConfig::large_icon_size, ClientGlobalConfig::large_icon_size));
    });

    connect(ui->view_file_with_tiling_action, &QAction::triggered, this, [this]() {
        this->ui->stacked_workspace_content_widget->setCurrentWidget(
            this->ui->workspace_content_list_view);
    });

    connect(ui->view_file_with_detail_action, &QAction::triggered, this, [this]() {
        this->ui->stacked_workspace_content_widget->setCurrentWidget(
            this->ui->workspace_content_table_view);
    });

    auto sort_file_by_name_func = [this]() {
        auto file_infos = ClientSingleton::get_cache_workspace_data_instance().get_file_infos(
            this->path_helper.get_workspace_path());
        if (file_infos.size() > 0) {
            std::sort(file_infos.begin(),
                      file_infos.end(),
                      [](const RemoteFileInfo& a, const RemoteFileInfo& b) {
                          return a.file_name < b.file_name;
                      });
        }
    };
    connect(ui->sort_file_by_name_action, &QAction::triggered, this, sort_file_by_name_func);

    auto sort_file_by_size_func = [this]() {
        auto file_infos = ClientSingleton::get_cache_workspace_data_instance().get_file_infos(
            this->path_helper.get_workspace_path());
        if (file_infos.size() > 0) {
            std::sort(file_infos.begin(),
                      file_infos.end(),
                      [](const RemoteFileInfo& a, const RemoteFileInfo& b) {
                          return a.file_size < b.file_size;
                      });
        }
    };
    connect(ui->sort_file_by_size_action, &QAction::triggered, this, sort_file_by_size_func);

    auto sort_file_by_type_func = [this]() {
        auto file_infos = ClientSingleton::get_cache_workspace_data_instance().get_file_infos(
            this->path_helper.get_workspace_path());
        if (file_infos.size() > 0) {
            std::sort(file_infos.begin(),
                      file_infos.end(),
                      [](const RemoteFileInfo& a, const RemoteFileInfo& b) {
                          return static_cast<uint32_t>(a.file_type) <
                                 static_cast<uint32_t>(b.file_type);
                      });
        }
    };
    connect(ui->sort_file_by_type_action, &QAction::triggered, this, sort_file_by_type_func);

    auto sort_file_by_time_func = [this]() {
        auto file_infos = ClientSingleton::get_cache_workspace_data_instance().get_file_infos(
            this->path_helper.get_workspace_path());
        if (file_infos.size() > 0) {
            std::sort(file_infos.begin(),
                      file_infos.end(),
                      [](const RemoteFileInfo& a, const RemoteFileInfo& b) {
                          return a.modify_time < b.modify_time;
                      });
        }
    };

    connect(ui->sort_file_by_modify_time_action, &QAction::triggered, this, sort_file_by_time_func);

    // for list/table view!
    connect(ui->workspace_content_table_view,
            &ElaTableView::customContextMenuRequested,
            this,
            &RemoteWorkspacePage::display_right_menu_for_table_view);

    connect(ui->workspace_content_list_view,
            &ElaListView::customContextMenuRequested,
            this,
            &RemoteWorkspacePage::display_right_menu_for_list_view);

    connect(right_menu->new_dir_action, &QAction::triggered, this, [this]() {
        this->new_dir_dialog->display();
    });


    connect(right_menu->upload_file_action, &QAction::triggered, this, [this]() {
        this->upload_file_impl();
    });

    connect(right_menu->refresh_action, &QAction::triggered, this, [this]() {
        this->refresh_workspace_content_impl();
    });

    connect(right_menu->small_icon_action, &QAction::triggered, this, [this]() {
        this->set_workspace_content_icon_size_impl(
            QSize(ClientGlobalConfig::small_icon_size, ClientGlobalConfig::small_icon_size));
    });

    connect(right_menu->middle_icon_action, &QAction::triggered, this, [this]() {
        this->set_workspace_content_icon_size_impl(
            QSize(ClientGlobalConfig::middle_icon_size, ClientGlobalConfig::middle_icon_size));
    });

    connect(right_menu->large_icon_action, &QAction::triggered, this, [this]() {
        this->set_workspace_content_icon_size_impl(
            QSize(ClientGlobalConfig::large_icon_size, ClientGlobalConfig::large_icon_size));
    });

    connect(right_menu->sort_by_name_action, &QAction::triggered, this, sort_file_by_name_func);
    connect(right_menu->sort_by_type_action, &QAction::triggered, this, sort_file_by_type_func);
    connect(right_menu->sort_by_size_action, &QAction::triggered, this, sort_file_by_size_func);
    connect(right_menu->sort_by_time_action, &QAction::triggered, this, sort_file_by_time_func);


    connect(right_menu->adjust_content_view_action,
            &QAction::triggered,
            this,
            &RemoteWorkspacePage::adjust_workspace_content_view);


    connect(right_menu->view_detail_action, &QAction::triggered, this, [this]() {
        ui->stacked_workspace_content_widget->setCurrentWidget(ui->workspace_content_table_view);
    });

    connect(right_menu->view_tiling_action, &QAction::triggered, this, [this]() {
        ui->stacked_workspace_content_widget->setCurrentWidget(ui->workspace_content_list_view);
    });


    connect(delete_file_dialog->ok_button, &ElaToolButton::clicked, this, [this]() {
        this->delete_file_dialog->close();
        this->delete_file_impl(this->delete_file_dialog->get_delete_fielname());
    });

    connect(delete_file_dialog->cancle_button, &ElaToolButton::clicked, this, [this]() {
        this->delete_file_dialog->close();
    });

    connect(rename_file_dialog->ok_button, &ElaToolButton::clicked, this, [this]() {
        auto dst_filename = rename_file_dialog->current_filename_line_edit->text();
        if (dst_filename.isEmpty()) {
            this->show_message("文件名不能为空!");
            return;
        }
        auto src_filename = rename_file_dialog->previous_filename_value->text();
        this->rename_file_dialog->close();
        this->rename_file_impl(src_filename, dst_filename);
    });

    connect(rename_file_dialog->cancle_button, &ElaToolButton::clicked, this, [this]() {
        this->rename_file_dialog->close();
    });

    connect(this, &RemoteWorkspacePage::start_delete_file, this, [this](int file_index) {
        auto& file_info = file_info_table_model->get_file_info(file_index);
        this->delete_file_dialog->hint_text->setText(
            QString("~~~是否要删除文件%1: '%2' ?\n~~~(请谨慎操作!!!😮😮😮)")
                .arg(file_info.file_type == FileKind::kFolder ? "夹" : "")
                .arg(file_info.file_name));
        this->delete_file_dialog->set_delete_filename(file_info.file_name);
        this->delete_file_dialog->display();
    });

    connect(this, &RemoteWorkspacePage::start_rename_file, this, [this](int file_index) {
        // qDebug() << "rename file...";
        auto& file_info = file_info_table_model->get_file_info(file_index);
        this->rename_file_dialog->previous_filename_value->setText(file_info.file_name);
        // QAQ
        this->rename_file_dialog->set_file_index(file_index);
        this->rename_file_dialog->display();
    });
}
}   // namespace client
}   // namespace tang