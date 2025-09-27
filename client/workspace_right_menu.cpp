#include "workspace_right_menu.h"

namespace tang {

namespace client {
WorkspaceRightMenu::WorkspaceRightMenu(QWidget* parent)
    : menu(new ElaMenu(parent)) {
    rename_action = menu->addAction("重命名");
    rename_action->setIcon(QIcon(":icons/images/rename.svg"));

    delete_action = menu->addAction("删除");
    delete_action->setIcon(QIcon(":icons/images/delete.svg"));

    refresh_action = menu->addAction("刷新");
    refresh_action->setIcon(QIcon(":icons/images/flush.svg"));

    share_action = menu->addAction("分享");
    share_action->setIcon(QIcon(":icons/images/share.svg"));

    small_icon_action = menu->addAction("小图标");
    small_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));

    middle_icon_action = menu->addAction("中图标");
    middle_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));

    large_icon_action = menu->addAction("大图标");
    large_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));

    view_detail_action = menu->addAction("详情");
    view_detail_action->setIcon(QIcon(":/icons/images/detail.svg"));

    view_tiling_action = menu->addAction("平铺");
    view_tiling_action->setIcon(QIcon(":icons/images/tiling.svg"));
}
}   // namespace client
}   // namespace tang