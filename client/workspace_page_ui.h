#pragma once
#include "ElaLineEdit.h"
#include "ElaListView.h"
#include "ElaScrollPage.h"
#include "ElaTableView.h"
#include "ElaToolButton.h"
#include "QStackedWidget"
#include <QHeaderView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include "ElaToggleSwitch.h"


namespace tang {
namespace client {
// here we should set the central widget and custom widget!
class RemoteWorkspacePageUi {
public:
    // action to upload file and make new dir!
    QAction* new_dir_action;
    QAction* upload_file_action;

    // some operator for the file,include cut/copy/rename/share/remove...
    ElaToolButton* cut_file_button;
    ElaToolButton* copy_file_button;
    ElaToolButton* rename_file_button;
    ElaToolButton* share_file_button;
    ElaToolButton* remove_file_button;

    // some sort options for the file content list!
    QAction* sort_file_by_name_action;
    QAction* sort_file_by_type_action;
    QAction* sort_file_by_size_action;
    QAction* sort_file_by_modify_time_action;
    QAction* sort_file_with_asce_order_action;
    QAction* sort_file_with_desce_order_action;

    // adjust the icon size of file!
    QAction* view_file_with_small_icon_action;
    QAction* view_file_with_middle_icon_action;
    QAction* view_file_with_large_icon_action;

    // the style to display the file content,include tile and detail!
    QAction* view_file_with_tiling_action;
    QAction* view_file_with_detail_action;

    // some button to control the view behavior!
    ElaToolButton* back_button;
    ElaToolButton* forward_button;
    ElaToolButton* parent_folder_button;
    ElaToolButton* flush_workspace_name_button;
    //whether show all workspaces!
    ElaToggleSwitch* show_all_workspaces;

    ElaLineEdit*   directory_line_edit;
    ElaLineEdit*   search_line_edit;
    ElaToolButton* search_button;

    ElaToolButton* view_tiling_button;
    ElaToolButton* view_detail_button;
    ElaToolButton* flush_workspace_content_button;

    ElaListView*    workspace_view;
    ElaListView*    workspace_content_list_view;
    ElaTableView*   workspace_content_table_view;
    QStackedWidget* stacked_workspace_content_widget;

    // the layout for the Page is title -> custom widget -> central widget!
    void setup_ui(ElaScrollPage* page);
};
}   // namespace client
}   // namespace tang