#include "display_pdf_file.h"
#include "ElaMessageBar.h"
#include "client_global_config.h"
#include "zoom_limit.h"
#include <QDebug>
#include <QPdfBookmarkModel>
#include <QPdfPageNavigator>



namespace tang {
namespace client {
DisplayPdf::DisplayPdf(QWidget* parent)
    : ElaWidget(parent)
    , ui(new DisplayPdfUi())
    , search_model(new QPdfSearchModel(parent))
    , document(new QPdfDocument(parent))
    , previous_search_text() {
    ui->setup_ui(this);
    this->setWindowTitle("sea");
    // this->moveToCenter();
    this->resize(QSize(900, 400));
    this->init_connects();
    this->setWindowOpacity(0.975);
}

DisplayPdf::~DisplayPdf() {
    delete ui;
}

void DisplayPdf::init_connects() {
    connect(
        ui->zoom_in_button, &ElaToolButton::clicked, this, &DisplayPdf::on_zoom_in_button_clicked);
    connect(ui->zoom_out_button,
            &ElaToolButton::clicked,
            this,
            &DisplayPdf::on_zoom_out_button_clicked);
    // skip to page if select!
    connect(ui->current_page, &ElaSpinBox::valueChanged, this, &DisplayPdf::page_selected);

    auto nav = ui->pdf_show_page->pageNavigator();
    // set the page if changed!
    connect(nav, &QPdfPageNavigator::currentPageChanged, ui->current_page, [this](int page) {
        this->ui->current_page->setValue(page + 1);
    });
    // if has history,enable the button!
    connect(nav,
            &QPdfPageNavigator::backAvailableChanged,
            ui->back_history_button,
            &ElaToolButton::setEnabled);
    connect(nav,
            &QPdfPageNavigator::forwardAvailableChanged,
            ui->forward_history_button,
            &ElaToolButton::setEnabled);

    // zoom selector!
    connect(ui->zoom_selector,
            &ZoomSelector::zoom_mode_changed,
            ui->pdf_show_page,
            &QPdfView::setZoomMode);
    connect(ui->zoom_selector,
            &ZoomSelector::zoom_factor_changed,
            ui->pdf_show_page,
            &QPdfView::setZoomFactor);
    ui->zoom_selector->reset();

    // bookmark
    QPdfBookmarkModel* bookmark_model = new QPdfBookmarkModel(this);
    bookmark_model->setDocument(document);
    ui->bookmark_view->setModel(bookmark_model);
    connect(ui->bookmark_view, &QAbstractItemView::activated, this, &DisplayPdf::bookmark_selected);

    // solve it by remove it to another thread!
    //  ui->thumbnail_view->setModel(document->pageModel());
    ui->pdf_show_page->setSearchModel(search_model);
    search_model->setDocument(document);

    // search
    connect(
        ui->search_button, &ElaToolButton::clicked, this, &DisplayPdf::on_search_button_clicked);
    connect(ui->continous_switch,
            &ElaToggleSwitch::toggled,
            this,
            &DisplayPdf::on_continous_switch_checked);


    ui->search_result_view->setModel(search_model);
    // not use the delegate!
    //  ui->search_result_view->setItemDelegate(new SearchResultDelegate(this));
    connect(ui->search_result_view->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &DisplayPdf::search_result_selected);


    // jump by thumbnail!
    connect(
        ui->thumbnail_view, &QListView::activated, this, &DisplayPdf::on_thumbnail_view_activated);

    ui->pdf_show_page->setDocument(document);
    // default is multi page
    ui->pdf_show_page->setPageMode(QPdfView::PageMode::MultiPage);

    // when the zoom factor changed,just set the zoom selector's text!
    connect(ui->pdf_show_page,
            &QPdfView::zoomFactorChanged,
            ui->zoom_selector,
            &ZoomSelector::set_zoom_factor);

    // open the file
    connect(ui->open_file_button,
            &ElaToolButton::clicked,
            this,
            &DisplayPdf::on_open_file_button_clicked);

    connect(ui->enable_thumbnail,
            &ElaToggleSwitch::toggled,
            this,
            &DisplayPdf::on_enable_thumbnail_checked);

    connect(ui->zoom_slider, &ElaSlider::valueChanged, this, &DisplayPdf::on_zoom_slider_changed);

    connect(ui->hide_left_button, &ElaToolButton::clicked, this, [this](bool hide) {
        if (hide) {
            // qDebug() << "hide";
            ui->tab_container->setMaximumWidth(12);
        } else {
            // qDebug() << "not hide";
            ui->tab_container->setMaximumWidth(320);
        }
    });
}


void DisplayPdf::show_message(const QString& message, bool error) {
    if (error) {
        ElaMessageBar::warning(ElaMessageBarType::TopLeft,
                               "Display PDF",
                               message,
                               ClientGlobalConfig::error_show_time,
                               this);
    } else {
        ElaMessageBar::success(ElaMessageBarType::TopLeft,
                               "Display PDF",
                               message,
                               ClientGlobalConfig::message_show_time,
                               this);
    }
}

double DisplayPdf::claim_zoom_factor(double factor) {
    if (factor > max_zoom_factor) {
        factor                       = max_zoom_factor;
        constexpr int max_factor_i32 = static_cast<int>(max_zoom_factor * 100);
        show_message(QString("已经达到最大缩放率啦 (%1%) 😂😂😂...").arg(max_factor_i32), false);
    } else if (factor < min_zoom_factor) {
        constexpr int min_factor_i32 = static_cast<int>(min_zoom_factor * 100);
        factor                       = min_zoom_factor;
        show_message(QString("已经达到最小缩放率啦 (%1%) 😮😮😮...").arg(min_factor_i32));
    }
    return factor;
}

double DisplayPdf::compute_zoom_factor(bool zoom_in) {
    constexpr double base_k = 1.107;
    const double     k      = zoom_in ? 1.0 / base_k : base_k;
    double           factor = ui->pdf_show_page->zoomFactor() * k;
    factor                  = this->claim_zoom_factor(factor);
    this->claim_zoom_factor(factor);
    return factor;
}

void DisplayPdf::on_zoom_in_button_clicked() {
    ui->pdf_show_page->setZoomFactor(compute_zoom_factor(true));
}

void DisplayPdf::on_zoom_out_button_clicked() {
    ui->pdf_show_page->setZoomFactor(compute_zoom_factor(false));
}

void DisplayPdf::bookmark_selected(const QModelIndex& index) {
    if (!index.isValid()) {
        ElaMessageBar::error(
            ElaMessageBarType::TopRight, "select bookmark", "invalid bookmark index", 2700, this);
    }
    int page = index.data(static_cast<int>(QPdfBookmarkModel::Role::Page)).toInt();
    // got the zoom level of bookmark!
    double zoom_level = index.data(static_cast<int>(QPdfBookmarkModel::Role::Level)).toReal();
    ui->pdf_show_page->pageNavigator()->jump(page, {}, zoom_level);
}


// attention,the page is start from 0
void DisplayPdf::page_selected(int page) {
    // qDebug() << "current page is " << page;
    int total_page = document->pageCount();
    if (page <= 0 || page > total_page) {
        // show_message(QString("页数:%1 不正确 🤣🤣🤣...").arg(page));
        return;
    }
    if (page == 1) {
        show_message("已经是第一页了 😂😂", false);
    } else if (page == total_page) {
        show_message("已经是最后一页了 😂😂😂", false);
    }
    page_jump(page - 1);
}


// to previous
void DisplayPdf::on_previous_page_triggered() {
    auto nav  = ui->pdf_show_page->pageNavigator();
    int  page = nav->currentPage();
    if (page == 0) {
        show_message("已经是第一页了 😂😂", false);
    } else {
        page--;
    }
    nav->jump(page, {}, nav->currentZoom());
}

void DisplayPdf::on_next_page_triggered() {
    auto nav  = ui->pdf_show_page->pageNavigator();
    int  page = nav->currentPage();
    if (page == document->pageCount() - 1) {
        show_message("已经是最后一页了 😂😂😂", false);

    } else {
        page++;
    }
    nav->jump(page, {}, nav->currentZoom());
}

void DisplayPdf::on_thumbnail_view_activated(const QModelIndex& index) {
    // auto nav = ui->pdf_show_page->pageNavigator();
    // nav->jump(index.row(), {}, nav->currentZoom());
    page_jump(index.row());
}


void DisplayPdf::on_back_button_clicked() {
    ui->pdf_show_page->pageNavigator()->back();
}

void DisplayPdf::on_forward_button_clicked() {
    ui->pdf_show_page->pageNavigator()->forward();
}


void DisplayPdf::on_find_previous_button_clicked() {
    int prev = ui->search_result_view->currentIndex().row() - 1;
    if (prev < 0) {
        return;
    }

    ui->search_result_view->setCurrentIndex(search_model->index(prev));
}

void DisplayPdf::on_find_next_button_clicked() {
    int next = ui->search_result_view->currentIndex().row() + 1;
    if (next >= search_model->rowCount({})) {
        next = 0;
    }
    ui->search_result_view->setCurrentIndex(search_model->index(next));
}

// jump if select the result
void DisplayPdf::search_result_selected(const QModelIndex& current, const QModelIndex& previous) {
    Q_UNUSED(previous);
    if (!current.isValid()) {
        return;
    }
    int page = current.data(static_cast<int>(QPdfSearchModel::Role::Page)).toInt();

    // find the location
    QPointF location = current.data(static_cast<int>(QPdfSearchModel::Role::Location)).toPointF();
    page_jump(page, location);
    ui->pdf_show_page->setCurrentSearchResultIndex(current.row());
}

void DisplayPdf::on_continous_switch_checked(bool checked) {
    // qDebug() << checked ;
    ui->pdf_show_page->setPageMode(checked ? QPdfView::PageMode::MultiPage
                                           : QPdfView::PageMode::SinglePage);
}

void DisplayPdf::on_search_button_clicked() {
    QString search_text = ui->search_line_edit->text();
    if (search_text == previous_search_text) {
        show_message("搜索内容没有任何变化 🤣🤣🤣");
        return;
    }
    if (search_text.isEmpty()) {
        return;
    }
    // qDebug() << "search text " << ui->search_line_edit->text();
    search_model->setSearchString(search_text);
    ui->switch_tabs->setCurrentWidget(ui->search_result_container);
    previous_search_text = search_text;
}

void DisplayPdf::clear() {
    /*clear
    /this is very important,if not reset the data,will get assert error when setting some style
    /int qt,the assert error is
    int QTreeViewPrivate::itemHeight(int item) const
    {
        Q_ASSERT(item < viewItems.size());
        ...
    }
    */
    ui->bookmark_view->reset();
    // ui->thumbnail_view->reset();
    ui->search_result_view->reset();
    ui->total_page->setText(QString("/   %1").arg(0));
    ui->current_page->setRange(0, 0);
    this->document->close();
}

void DisplayPdf::process_after_load() {
    if (document->error() == QPdfDocument::Error::None) {
        show_message("成功打开pdf文件 😊😊😊...", false);
        // set the page
        page_jump(0);
        int total_page = document->pageCount();
        ui->total_page->setText(QString("/   %1").arg(total_page));
        ui->current_page->setRange(1, total_page);
    } else {
        show_message("打开pdf文件失败 ψ(._. )>");
        document->close();
    }
}

void DisplayPdf::load_pdf(const QString& file_path) {
    // invoke this if last success!
    this->clear();
    document->load(file_path);
    process_after_load();
}

void DisplayPdf::load_pdf(QIODevice* device) {
    this->clear();
    document->load(device);
    process_after_load();
}

void DisplayPdf::on_open_file_button_clicked() {
    this->clear();

    QString file_name = QFileDialog::getOpenFileName(this, "打开PDF文件", "", "PDF Files (*.pdf)");
    if (file_name.isEmpty()) {
        show_message("未选择文件 😮😮😮...");
        return;
    }
    this->load_pdf(file_name);
}

void DisplayPdf::page_jump(int jump_to, const QPointF& loc) {
    auto nav = ui->pdf_show_page->pageNavigator();
    nav->jump(jump_to, loc, nav->currentZoom());
}

void DisplayPdf::on_enable_thumbnail_checked(bool checked) {
    auto index = ui->switch_tabs->indexOf(ui->thumbnail_container);
    ui->switch_tabs->setTabEnabled(index, checked);
}

void DisplayPdf::on_zoom_slider_changed(int value) {
    double zoom_factor = static_cast<double>(value) / 100.0;
    zoom_factor        = this->claim_zoom_factor(zoom_factor);
    // zoom_factor = compute_zoom_factor()
    ui->pdf_show_page->setZoomFactor(zoom_factor);
}


}   // namespace client
}   // namespace tang