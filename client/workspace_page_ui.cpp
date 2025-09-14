#include "workspace_page_ui.h"
#include <ElaScrollPage.h>


namespace tang {
namespace client {
void RemoteWorkspacePageUi::setup_ui(ElaScrollPage* page) {
    // icon size for tool bar and other icons...
    constexpr QSize tool_bar_icon_size(18, 18);
    constexpr QSize other_icon_size(16, 16);

    QWidget*     central_widget = new QWidget(page);
    QVBoxLayout* central_layout = new QVBoxLayout(central_widget);
    // set the tool bar!
    ElaToolBar* tool_bar = new ElaToolBar(page);
    tool_bar->setIconSize(tool_bar_icon_size);
    central_layout->addWidget(tool_bar);

    ElaToolButton* new_something_button = new ElaToolButton(page);
    new_something_button->setText("新建");
    // new_something_button->setElaIcon(ElaIconType::BatterySlash);
    new_something_button->setIcon(QIcon(":icons/images/better_new_something.svg"));
    new_something_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tool_bar->addWidget(new_something_button);

    ElaMenu* new_something_menu = new ElaMenu(page);
    new_dir_action              = new_something_menu->addAction("文件夹");
    new_dir_action->setIcon(QIcon(":icons/images/better_folder.svg"));
    // here we should modify the icon!
    upload_file_action = new_something_menu->addAction("上传文件");
    upload_file_action->setIcon(QIcon(":icons/images/upload.svg"));
    new_something_button->setMenu(new_something_menu);


    cut_file_button = new ElaToolButton(page);
    cut_file_button->setToolTip("剪切");
    // cut_file_button->setElaIcon(ElaIconType::CalendarCircleUser);
    cut_file_button->setIcon(QIcon(":icons/images/cut.svg"));
    tool_bar->addWidget(cut_file_button);

    copy_file_button = new ElaToolButton(page);
    copy_file_button->setToolTip("复制");
    // copy_file_button->setElaIcon(ElaIconType::Copy);
    copy_file_button->setIcon(QIcon(":icons/images/copy.svg"));
    tool_bar->addWidget(copy_file_button);

    rename_file_button = new ElaToolButton(page);
    rename_file_button->setToolTip("重命名");
    // rename_file_button->setElaIcon(ElaIconType::HandBackPointRight);
    rename_file_button->setIcon(QIcon(":icons/images/rename.svg"));
    tool_bar->addWidget(rename_file_button);

    share_file_button = new ElaToolButton(page);
    share_file_button->setToolTip("分享");
    // share_file_button->setElaIcon(ElaIconType::Share);
    share_file_button->setIcon(QIcon(":icons/images/share.svg"));
    tool_bar->addWidget(share_file_button);


    ElaToolButton* remove_file_button = new ElaToolButton(page);
    remove_file_button->setToolTip("删除");
    // remove_file_button->setElaIcon(ElaIconType::DeleteRight);
    remove_file_button->setIcon(QIcon(":icons/images/delete.svg"));
    tool_bar->addWidget(remove_file_button);

    ElaToolButton* sort_file_button = new ElaToolButton(page);
    sort_file_button->setText("排序");
    // sort_file_button->setElaIcon(ElaIconType::Sort);
    sort_file_button->setIcon(QIcon(":icons/images/sort.svg"));
    sort_file_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    // add menu
    ElaMenu* sort_menu       = new ElaMenu(page);
    sort_file_by_name_action = sort_menu->addAction("名称");
    sort_file_by_name_action->setIcon(QIcon(":icons/images/file_name.svg"));

    sort_file_by_type_action = sort_menu->addAction("类型");
    sort_file_by_type_action->setIcon(QIcon(":icons/images/green_leaf.svg"));

    sort_file_by_size_action = sort_menu->addAction("大小");
    sort_file_by_size_action->setIcon(QIcon(":icons/images/butterfly.svg"));

    sort_file_by_modify_time_action = sort_menu->addAction("修改时间");
    sort_file_by_modify_time_action->setIcon(QIcon(":icons/images/flower.svg"));
    sort_menu->addSeparator();

    sort_file_with_asce_order_action = sort_menu->addAction("升序");
    sort_file_with_asce_order_action->setIcon(QIcon(":icons/images/asce.svg"));

    sort_file_with_desce_order_action = sort_menu->addAction("降序");
    sort_file_with_desce_order_action->setIcon(QIcon(":icons/images/desce.svg"));
    sort_file_button->setMenu(sort_menu);
    tool_bar->addWidget(sort_file_button);

    // add a seperator between sort and view!
    tool_bar->addSeparator();

    ElaToolButton* view_button = new ElaToolButton(page);
    view_button->setText("查看");
    // view_button->setElaIcon(ElaIconType::GlassHalf);
    view_button->setIcon(QIcon(":icons/images/view_file.svg"));
    view_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ElaMenu* view_menu               = new ElaMenu(page);
    view_file_with_small_icon_action = view_menu->addAction("小图标");
    view_file_with_small_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));
    view_file_with_middle_icon_action = view_menu->addAction("中图标");
    view_file_with_middle_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));

    view_file_with_large_icon_action = view_menu->addAction("大图标");
    view_file_with_large_icon_action->setIcon(QIcon(":icons/images/leaf.svg"));

    view_menu->addSeparator();
    view_file_with_tiling_action = view_menu->addAction("平铺");
    view_file_with_tiling_action->setIcon(QIcon(":icons/images/tiling.svg"));

    view_file_with_detail_action = view_menu->addAction("详情");
    view_file_with_detail_action->setIcon(QIcon(":/icons/images/detail.svg"));
    view_button->setMenu(view_menu);
    tool_bar->addWidget(view_button);

    // other content
    QWidget* navigation_widget = new QWidget(page);
    navigation_widget->setMaximumHeight(36);
    central_layout->addWidget(navigation_widget);
    QHBoxLayout* navigation_layout = new QHBoxLayout(navigation_widget);
    navigation_layout->setContentsMargins({0, 0, 0, 0});

    back_button = new ElaToolButton(page);
    // back_button->setElaIcon(ElaIconType::Backpack);
    back_button->setIcon(QIcon(":icons/images/simple_back.svg"));
    back_button->setIconSize(other_icon_size);
    back_button->setToolTip("后退");
    navigation_layout->addWidget(back_button);

    forward_button = new ElaToolButton(page);
    // forward_button->setElaIcon(ElaIconType::Forward);
    forward_button->setIcon(QIcon(":icons/images/simple_forward.svg"));
    forward_button->setIconSize(other_icon_size);
    forward_button->setToolTip("前进");
    navigation_layout->addWidget(forward_button);

    parent_folder_button = new ElaToolButton(page);
    // parent_folder_button->setElaIcon(ElaIconType::NfcMagnifyingGlass);
    parent_folder_button->setIcon(QIcon(":icons/images/parent_dir.svg"));
    parent_folder_button->setIconSize(other_icon_size);
    parent_folder_button->setToolTip("上级目录");
    navigation_layout->addWidget(parent_folder_button);


    flush_button = new ElaToolButton(page);
    // flush_button->setElaIcon(ElaIconType::FaceFlushed);
    flush_button->setIcon(QIcon(":icons/images/flush.svg"));
    flush_button->setToolTip("刷新");
    flush_button->setIconSize(other_icon_size);
    navigation_layout->addWidget(flush_button);

    // navigation_layout->addStretch();

    // line edit!
    directory_line_edit = new ElaLineEdit(page);
    navigation_layout->addWidget(directory_line_edit);

    navigation_layout->addSpacing(8);

    search_line_edit = new ElaLineEdit(page);
    search_line_edit->setPlaceholderText("搜索");
    search_line_edit->setMaximumWidth(120);
    navigation_layout->addWidget(search_line_edit);

    search_button = new ElaToolButton(page);
    search_button->setToolTip("搜索");
    search_button->setIcon(QIcon(":icons/images/search_blue.svg"));
    search_button->setIconSize(other_icon_size);
    navigation_layout->addWidget(search_button);

    view_tiling_button = new ElaToolButton(page);
    // view_tiling_button->setElaIcon(ElaIconType::Tilde);
    view_tiling_button->setIcon(QIcon(":icons/images/tiling.svg"));
    view_tiling_button->setIconSize(other_icon_size);
    view_tiling_button->setToolTip("平铺");
    navigation_layout->addWidget(view_tiling_button);


    view_detai_button = new ElaToolButton(page);
    // view_detai_button->setElaIcon(ElaIconType::Democrat);
    view_detai_button->setIcon(QIcon(":icons/images/detail.svg"));
    view_detai_button->setIconSize(other_icon_size);
    view_detai_button->setToolTip("详情");
    navigation_layout->addWidget(view_detai_button);

    // content
    QSplitter* workspace_spliter = new QSplitter(page);
    workspace_spliter->setOrientation(Qt::Horizontal);
    workspace_spliter->setOpaqueResize(true);
    workspace_spliter->setChildrenCollapsible(false);
    central_layout->addWidget(workspace_spliter);

    workspace_view = new ElaListView(page);
    workspace_view->setMinimumWidth(100);
    workspace_view->setMaximumWidth(160);
    workspace_view->setIconSize(QSize(36, 36));
    workspace_view->setSpacing(8);
    workspace_spliter->addWidget(workspace_view);


    // QWidget* workspace_content_widget = new QWidget(page);
    // workspace_spliter->addWidget(workspace_content_widget);
    // QVBoxLayout* workspace_content_layout = new QVBoxLayout(workspace_content_widget);
    // workspace_content_layout->setContentsMargins({0, 0, 0, 0});

    stacked_workspace_content_widget = new QStackedWidget(page);
    workspace_spliter->addWidget(stacked_workspace_content_widget);

    workspace_content_list_view = new ElaListView(page);
    workspace_content_list_view->setIconSize(QSize(36, 36));
    workspace_content_list_view->setResizeMode(ElaListView::Adjust);
    workspace_content_list_view->setWrapping(true);
    workspace_content_list_view->setSpacing(5);
    stacked_workspace_content_widget->addWidget(workspace_content_list_view);

    workspace_content_table_view = new ElaTableView(page);
    workspace_content_table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    workspace_content_table_view->setAlternatingRowColors(true);
    workspace_content_table_view->setIconSize(QSize(21, 21));
    workspace_content_table_view->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    workspace_content_table_view->verticalHeader()->setHidden(true);
    // workspace_content_table_view->horizontalHeader()->setSectionResizeMode(
    //     QHeaderView::Interactive);
    workspace_content_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    workspace_content_table_view->horizontalHeader()->setMinimumSectionSize(60);
    workspace_content_table_view->verticalHeader()->setMinimumSectionSize(48);
    stacked_workspace_content_widget->addWidget(workspace_content_table_view);

    stacked_workspace_content_widget->setCurrentIndex(1);

    // the top title is very important!
    central_widget->setWindowTitle("workspace");
    page->addCentralWidget(central_widget);
}
}   // namespace client
}   // namespace tang