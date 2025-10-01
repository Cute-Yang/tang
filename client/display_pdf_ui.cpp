#include "display_pdf_ui.h"
#include "zoom_limit.h"

namespace tang {
namespace client {
void DisplayPdfUi::setup_ui(QWidget* window) {
    QVBoxLayout* window_layout = new QVBoxLayout(window);

    QWidget* tool_bar_container = new QWidget(window);
    tool_bar_container->setMaximumHeight(80);
    QHBoxLayout* tool_bar_container_layout = new QHBoxLayout(tool_bar_container);

    tool_bar = new ElaToolBar(window);
    tool_bar->setIconSize(QSize(21, 21));
    hide_left_button = new ElaToolButton(window);
    hide_left_button->setCheckable(true);
    // means wheter hide?
    hide_left_button->setChecked(false);
    hide_left_button->setToolTip(QString("收起侧边栏"));
    // hide_left_button->setElaIcon(ElaIconType::ArrowsSplitUpAndLeft);
    hide_left_button->setIcon(QIcon(":/icons/images/shanhu.svg"));
    tool_bar->addWidget(hide_left_button);

    zoom_in_button = new ElaToolButton(window);
    zoom_in_button->setToolTip(QString("缩小"));
    // zoom_in_button->setElaIcon(ElaIconType::BadgeCheck);
    zoom_in_button->setIcon(QIcon(":/icons/images/zoom_in.svg"));
    tool_bar->addWidget(zoom_in_button);

    zoom_selector = new ZoomSelector(window);
    tool_bar->addWidget(zoom_selector);

    zoom_out_button = new ElaToolButton(window);
    zoom_out_button->setToolTip("放大");
    // zoom_out_button->setElaIcon(ElaIconType::AnchorCircleXmark);
    zoom_out_button->setIcon(QIcon(":icons/images/zoom_out.svg"));
    tool_bar->addWidget(zoom_out_button);
    tool_bar->addSeparator();

    back_history_button = new ElaToolButton(window);
    back_history_button->setToolTip("上一个浏览");
    // back_history_button->setElaIcon(ElaIconType::ChartSimpleHorizontal);
    back_history_button->setIcon(QIcon(":/icons/images/FastBackward.svg"));
    tool_bar->addWidget(back_history_button);

    current_page = new ElaSpinBox(window);
    // current_page->setRange(0,std::numeric_limits<uint16_t>::max());
    current_page->setRange(0, 0);
    auto font = current_page->font();
    font.setFamily("微软雅黑");
    font.setPointSize(10);
    current_page->setFont(font);
    current_page->setMaximumWidth(30);
    tool_bar->addWidget(current_page);

    total_page = new ElaText(window);
    total_page->setText(" /  (总页数)");
    total_page->setFont(font);
    tool_bar->addWidget(total_page);


    forward_history_button = new ElaToolButton(window);
    forward_history_button->setToolTip("下一个浏览");
    // forward_history_button->setElaIcon(ElaIconType::CarWrench);
    forward_history_button->setIcon(QIcon(":/icons/images/fast-forward.svg"));
    tool_bar->addWidget(forward_history_button);
    tool_bar->addSeparator();

    open_file_button = new ElaToolButton(window);
    open_file_button->setToolTip("浏览本地文件");
    // open_file_button->setElaIcon(ElaIconType::EarthAfrica);
    open_file_button->setIcon(QIcon(":/icons/images/green_file.svg"));
    tool_bar->addWidget(open_file_button);

    tool_bar_container_layout->addWidget(tool_bar);
    tool_bar_container_layout->addStretch();


    // add a text!
    conitnous_text = new ElaText("单/多页浏览");
    //  font = conitnous_text->font();
    conitnous_text->setFont(font);
    tool_bar_container_layout->addWidget(conitnous_text);

    continous_switch = new ElaToggleSwitch(window);
    continous_switch->setToolTip(QString("连续翻页"));
    continous_switch->setIsToggled(true);
    tool_bar_container_layout->addWidget(continous_switch);


    zoom_slider = new ElaSlider(window);
    tool_bar_container_layout->addWidget(zoom_slider);
    zoom_slider->setRange(0, static_cast<int>(max_zoom_factor * 100));
    zoom_slider->setToolTip("缩放视图");
    zoom_slider->setMaximumWidth(120);


    window_layout->addWidget(tool_bar_container);

    // add tabs and page!
    QWidget* page_container = new QWidget(window);
    // set the layout for container!
    QVBoxLayout* page_container_layout = new QVBoxLayout(page_container);

    QSplitter* page_spliter = new QSplitter(window);
    // set to horizontal!
    page_spliter->setOrientation(Qt::Horizontal);
    page_container_layout->addWidget(page_spliter);

    // tabs!
    tab_container = new QWidget(window);
    tab_container->setMaximumWidth(320);
    // set the vertical layout!
    QVBoxLayout* tab_container_layout = new QVBoxLayout(tab_container);


    // add the search!
    QWidget*     search_line_container = new QWidget(window);
    QHBoxLayout* search_line_layout    = new QHBoxLayout(search_line_container);

    // add search!
    search_line_edit = new ElaLineEdit(window);
    search_line_edit->setPlaceholderText(QString("搜索文档"));
    search_line_edit->setMinimumWidth(120);
    search_line_edit->setMaximumWidth(600);

    search_button = new ElaToolButton(window);
    search_button->setToolTip("搜索");
    // search_button->setElaIcon(ElaIconType::EarListen);
    search_button->setIcon(QIcon(":/icons/images/search_blue.svg"));
    search_button->setIconSize(QSize(21, 21));

    search_line_layout->addStretch();
    search_line_layout->addWidget(search_line_edit);
    search_line_layout->addSpacing(4);
    search_line_layout->addWidget(search_button);
    search_line_layout->addStretch();

    enable_thumbnail_text = new ElaText(window);
    enable_thumbnail_text->setText(QString("查看缩略图"));
    enable_thumbnail_text->setFont(font);
    search_line_layout->addWidget(enable_thumbnail_text);
    search_line_layout->addSpacing(4);

    enable_thumbnail = new ElaToggleSwitch(window);
    // enable_thumbnail->setEnabled(false);
    enable_thumbnail->setToolTip("启用缩略图");
    search_line_layout->addWidget(enable_thumbnail);
    tab_container_layout->addWidget(search_line_container);

    // add tabs!
    switch_tabs = new ElaTabWidget(window);
    tab_container_layout->addWidget(switch_tabs);

    QMargins    view_margin = {3, 3, 3, 3};
    QSizePolicy size_policy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(switch_tabs->hasHeightForWidth());
    switch_tabs->setSizePolicy(size_policy);
    switch_tabs->setDocumentMode(false);

    // book
    bookmark_view = new ElaTreeView(window);
    size_policy.setHeightForWidth(bookmark_view->sizePolicy().hasHeightForWidth());
    bookmark_view->setSizePolicy(size_policy);
    bookmark_view->setHeaderHidden(true);
    QWidget*     bookmark_container = new QWidget(window);
    QVBoxLayout* bookmark_layout    = new QVBoxLayout(bookmark_container);
    bookmark_layout->addWidget(bookmark_view);
    bookmark_layout->setContentsMargins(view_margin);
    switch_tabs->addTab(bookmark_container, QString("书签"));

    // search result!
    search_result_view = new ElaListView(window);
    search_result_view->setResizeMode(QListView::Adjust);
    search_result_container = new QWidget(window);

    QVBoxLayout* search_result_layout = new QVBoxLayout(search_result_container);
    search_result_layout->setContentsMargins(view_margin);
    search_result_layout->addWidget(search_result_view);

    switch_tabs->addTab(search_result_container, "搜索结果");

    // image for large file,it is slow!
    thumbnail_view = new QListView(window);
    size_policy.setHeightForWidth(thumbnail_view->sizePolicy().hasHeightForWidth());
    thumbnail_view->setSizePolicy(size_policy);
    thumbnail_view->setIconSize(QSize(128, 128));
    // thumbnail_view->setMovement(QListView::Static);
    thumbnail_view->setResizeMode(QListView::Adjust);
    thumbnail_view->setViewMode(QListView::IconMode);
    thumbnail_container           = new QWidget(window);
    QVBoxLayout* thumbnail_layout = new QVBoxLayout(thumbnail_container);
    thumbnail_layout->setContentsMargins(view_margin);
    thumbnail_layout->addWidget(thumbnail_view);
    switch_tabs->addTab(thumbnail_container, QString("缩略图"));

    switch_tabs->setTabEnabled(switch_tabs->indexOf(thumbnail_container), false);

    page_spliter->addWidget(tab_container);
    // page
    pdf_show_page = new QPdfView(window);
    page_spliter->addWidget(pdf_show_page);
    window_layout->addWidget(page_container);

    zoom_selector->setFont(font);
}
}   // namespace client
}   // namespace tang