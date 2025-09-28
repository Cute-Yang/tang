#include "workspace_right_menu.h"

namespace tang {

namespace client {
WorkspaceRightMenu::WorkspaceRightMenu(QWidget* parent)
    : menu(new ElaMenu(parent)) {
    new_dir_action = menu->addAction("新建文件夹");
    new_dir_action->setIcon(QIcon(":icons/images/new_dir.svg"));
    rename_action = menu->addAction("重命名");
    rename_action->setIcon(QIcon(":icons/images/rename.svg"));

    delete_action = menu->addAction("删除");
    delete_action->setIcon(QIcon(":icons/images/delete.svg"));

    refresh_action = menu->addAction("刷新");
    refresh_action->setIcon(QIcon(":icons/images/color_refresh.svg"));

    share_action = menu->addAction("分享");
    share_action->setIcon(QIcon(":icons/images/share.svg"));

    ElaMenu* icon_menu = new ElaMenu("图标", parent);
    icon_menu->setIcon(QIcon(":icons/images/feng_leaf.svg"));

    small_icon_action = icon_menu->addAction("小图标");
    small_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));

    middle_icon_action = icon_menu->addAction("中图标");
    middle_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));

    large_icon_action = icon_menu->addAction("大图标");
    large_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));
    menu->addMenu(icon_menu);

    ElaMenu* view_menu = new ElaMenu("查看方式", parent);
    view_menu->setIcon(QIcon(":icons/images/cloud.svg"));
    menu->addMenu(view_menu);

    ElaMenu* sort_menu = new ElaMenu("排序", parent);
    sort_menu->setIcon(QIcon(":icons/images/sort.svg"));
    sort_by_name_action = sort_menu->addAction("名称");
    sort_by_name_action->setIcon(QIcon(":icons/images/file_name.svg"));

    sort_by_type_action = sort_menu->addAction("类型");
    sort_by_type_action->setIcon(QIcon(":icons/images/green_leaf.svg"));

    sort_by_size_action = sort_menu->addAction("大小");
    sort_by_size_action->setIcon(QIcon(":icons/images/butterfly.svg"));

    sort_by_time_action = sort_menu->addAction("修改时间");
    sort_by_time_action->setIcon(QIcon(":icons/images/flower.svg"));
    menu->addMenu(sort_menu);

    view_detail_action = view_menu->addAction("详情");
    view_detail_action->setIcon(QIcon(":/icons/images/detail.svg"));

    view_tiling_action = view_menu->addAction("平铺");
    view_tiling_action->setIcon(QIcon(":icons/images/tiling.svg"));
}
}   // namespace client
}   // namespace tang