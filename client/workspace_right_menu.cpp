#include "workspace_right_menu.h"

namespace tang {

namespace client {
WorkspaceRightMenu::WorkspaceRightMenu(QWidget* parent)
    : menu(new ElaMenu(parent)) {
    new_dir_action = menu->addAction("新建文件夹");
    new_dir_action->setIcon(QIcon(":icons/images/new_dir.svg"));
    rename_action  = menu->addAction("重命名");
    rename_action->setIcon(QIcon(":icons/images/rename.svg"));

    delete_action = menu->addAction("删除");
    delete_action->setIcon(QIcon(":icons/images/delete.svg"));

    refresh_action = menu->addAction("刷新");
    refresh_action->setIcon(QIcon(":icons/images/color_refresh.svg"));

    share_action = menu->addAction("分享");
    share_action->setIcon(QIcon(":icons/images/share.svg"));

    ElaMenu* icon_menu = new ElaMenu(parent);
    icon_menu->setIcon(QIcon(":icons/images/feng_leaf.svg"));

    small_icon_action = icon_menu->addAction("小图标");
    small_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));

    middle_icon_action = icon_menu->addAction("中图标");
    middle_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));

    large_icon_action = icon_menu->addAction("大图标");
    large_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));
    menu->addMenu(icon_menu);

    ElaMenu* view_menu = new ElaMenu(parent);
    view_menu->setIcon(QIcon("ico"))


    view_detail_action = menu->addAction("详情");
    view_detail_action->setIcon(QIcon(":/icons/images/detail.svg"));

    view_tiling_action = menu->addAction("平铺");
    view_tiling_action->setIcon(QIcon(":icons/images/tiling.svg"));
}
}   // namespace client
}   // namespace tang