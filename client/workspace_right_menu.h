#pragma once
#include "ElaMenu.h"
#include <QAction>

namespace tang {
namespace client {
class WorkspaceRightMenu {
public:
    ElaMenu* menu;
    QAction* new_dir_action;
    QAction* rename_action;
    QAction* delete_action;
    QAction* refresh_action;
    QAction* share_action;
    //icon
    QAction* small_icon_action;
    QAction* middle_icon_action;
    QAction* large_icon_action;
    
    //sort
    QAction* sort_by_name_action;
    QAction* sort_by_type_action;
    QAction* sort_by_size_action;
    QAction* sort_by_time_action;

    //view
    QAction* view_detail_action;
    QAction* view_tiling_action;
    WorkspaceRightMenu(QWidget* parent = nullptr);
};

}   // namespace client
}   // namespace tang