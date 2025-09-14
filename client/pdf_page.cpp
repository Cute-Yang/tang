#include "pdf_page.h"
#include "ElaMessageBar.h"
#include "pdf_ui.h"
// #include "search_result_delegate.h"
#include "zoom_limit.h"
#include <QDebug>
#include <QPdfBookmarkModel>
#include <QPdfPageNavigator>

namespace tang {
namespace client {
SeaPdf::SeaPdf(QWidget* parent)
    : ElaWidget(parent)
    , ui(new SeaPdfUi())
    , search_model(new QPdfSearchModel(parent))
    , document(new QPdfDocument(parent))
    , previous_search_text() {
    ui->setup_ui(this);
    this->setWindowTitle("sea");
    // this->moveToCenter();
    this->resize(QSize(900, 400));
    this->init_connects();

    // setAutoFillBackground(true);    // 这句要加上, 否则可能显示不出背景图.
    // QPalette palette = this->palette();
    // //palette.setColor(QPalette::Window, Qt::red);  // 设置背景色
    // //palette.setBrush(this->backgroundRole(), Qt::black);// 设置背景色
    // palette.setBrush(QPalette::Window,
    //         QBrush(QPixmap(":icons/images/small_green_flower.png").scaled(    // 缩放背景图.
    //             this->size(),
    //             Qt::IgnoreAspectRatio,
    //             Qt::SmoothTransformation)));    // 使用平滑的缩放方式
    // this->setPalette(palette);
    this->setWindowOpacity(0.975);    
}


void SeaPdf::init_connects() {
    connect(ui->zoom_in_button, &ElaToolButton::clicked, this, &SeaPdf::zoom_in_clicked);
    connect(ui->zoom_out_button, &ElaToolButton::clicked, this, &SeaPdf::zoom_out_clicked);

    // skip to page if select!
    connect(ui->current_page, &ElaSpinBox::valueChanged, this, &SeaPdf::page_selected);

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
    connect(ui->bookmark_view, &QAbstractItemView::activated, this, &SeaPdf::bookmark_selected);

    ui->thumbnail_view->setModel(document->pageModel());
    ui->pdf_show_page->setSearchModel(search_model);
    search_model->setDocument(document);

    // search
    connect(ui->search_button, &ElaToolButton::clicked, this, &SeaPdf::search_button_clicked);
    connect(
        ui->continous_switch, &ElaToggleSwitch::toggled, this, &SeaPdf::continous_switch_checked);


    ui->search_result_view->setModel(search_model);
    // not use the delegate!
    //  ui->search_result_view->setItemDelegate(new SearchResultDelegate(this));
    connect(ui->search_result_view->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &SeaPdf::search_result_selected);


    // jump by thumbnail!
    connect(ui->thumbnail_view, &QListView::activated, this, &SeaPdf::thumbnail_view_activated);

    ui->pdf_show_page->setDocument(document);
    // default is multi page
    ui->pdf_show_page->setPageMode(QPdfView::PageMode::MultiPage);

    // when the zoom factor changed,just set the zoom selector's text!
    connect(ui->pdf_show_page,
            &QPdfView::zoomFactorChanged,
            ui->zoom_selector,
            &ZoomSelector::set_zoom_factor);

    // open the file
    connect(ui->open_file_button, &ElaToolButton::clicked, this, &SeaPdf::open_file_clicked);

    connect(
        ui->enable_thumbnail, &ElaToggleSwitch::toggled, this, &SeaPdf::enable_thumbnail_checked);

    connect(ui->zoom_slider, &ElaSlider::valueChanged, this, &SeaPdf::zoom_slider_changed);
}


double SeaPdf::claim_zoom_factor(double factor) {
    QString show_message;

    if (factor > max_zoom_factor) {
        factor                       = max_zoom_factor;
        constexpr int max_factor_i32 = static_cast<int>(max_zoom_factor * 100);
        show_message                 = QString("已经达到最大缩放率啦 (%1%)").arg(max_factor_i32);
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "zoom out", show_message, 2700, this);
    } else if (factor < min_zoom_factor) {
        constexpr int min_factor_i32 = static_cast<int>(min_zoom_factor * 100);
        factor                       = min_zoom_factor;
        show_message                 = QString("已经达到最小缩放率啦 (%1%)").arg(min_factor_i32);
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "zoom in", show_message, 2700, this);
    }
    return factor;
}

double SeaPdf::compute_zoom_factor(bool zoom_in) {
    constexpr double base_k = 1.107;
    const double     k      = zoom_in ? 1.0 / base_k : base_k;
    double           factor = ui->pdf_show_page->zoomFactor() * k;
    factor = this->claim_zoom_factor(factor);
    this->claim_zoom_factor(factor);
    // qDebug() << "get zoom factor -> " << factor;
    return factor;
}

void SeaPdf::zoom_in_clicked() {
    ui->pdf_show_page->setZoomFactor(compute_zoom_factor(true));
}

void SeaPdf::zoom_out_clicked() {
    ui->pdf_show_page->setZoomFactor(compute_zoom_factor(false));
}

void SeaPdf::bookmark_selected(const QModelIndex& index) {
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
void SeaPdf::page_selected(int page) {
    // qDebug() << "current page is " << page;
    int total_page = document->pageCount();
    if (page <= 0 || page > total_page) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "page select",
                               QString("无效的页 %1").arg(page),
                               2700,
                               this);
    }
    if (page == 1) {
        ElaMessageBar::information(
            ElaMessageBarType::TopRight, "page select", "已经是第一页啦", 2700, this);
    } else if (page == total_page) {
        ElaMessageBar::information(
            ElaMessageBarType::TopRight, "page select", "已经是最后一页啦", 2700, this);
    }
    page_jump(page - 1);
}


// to previous
void SeaPdf::previous_page_triggered() {
    auto nav  = ui->pdf_show_page->pageNavigator();
    int  page = nav->currentPage();
    if (page == 0) {
        // already the first!
        ElaMessageBar::warning(
            ElaMessageBarType::TopRight, "skip page", "已经是第一页啦", 2700, this);
    } else {
        page--;
    }
    nav->jump(page, {}, nav->currentZoom());
}

void SeaPdf::next_page_triggered() {
    auto nav  = ui->pdf_show_page->pageNavigator();
    int  page = nav->currentPage();
    if (page == document->pageCount() - 1) {
        // already the first!
        ElaMessageBar::warning(
            ElaMessageBarType::TopRight, "skip page", "已经是最后一页啦", 2700, this);
    } else {
        page++;
    }
    nav->jump(page, {}, nav->currentZoom());
}

void SeaPdf::thumbnail_view_activated(const QModelIndex& index) {
    // auto nav = ui->pdf_show_page->pageNavigator();
    // nav->jump(index.row(), {}, nav->currentZoom());
    page_jump(index.row());
}


void SeaPdf::back_clicked() {
    ui->pdf_show_page->pageNavigator()->back();
}

void SeaPdf::forward_clicked() {
    ui->pdf_show_page->pageNavigator()->forward();
}


void SeaPdf::find_previous_clicked() {
    int prev = ui->search_result_view->currentIndex().row() - 1;
    if (prev < 0) {
        prev = search_model->rowCount({}) - 1;
    }
    if (prev >= 0) {
        ui->search_result_view->setCurrentIndex(search_model->index(prev));
    }
}

void SeaPdf::find_next_clicked() {
    int next = ui->search_result_view->currentIndex().row() + 1;
    if (next >= search_model->rowCount({})) {
        next = 0;
    }
    ui->search_result_view->setCurrentIndex(search_model->index(next));
}

// jump if select the result
void SeaPdf::search_result_selected(const QModelIndex& current, const QModelIndex& previous) {
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

void SeaPdf::continous_switch_checked(bool checked) {
    // qDebug() << checked ;
    ui->pdf_show_page->setPageMode(checked ? QPdfView::PageMode::MultiPage
                                           : QPdfView::PageMode::SinglePage);
}

void SeaPdf::search_button_clicked() {
    QString search_text = ui->search_line_edit->text();
    if (search_text == previous_search_text) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "search",
                               "the search content is not changed!",
                               2700,
                               this);
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


void SeaPdf::open_file_clicked() {
    QString file_name = QFileDialog::getOpenFileName(this, "打开PDF文件", "", "PDF Files (*.pdf)");
    if (file_name.isEmpty()) {
        ElaMessageBar::warning(
            ElaMessageBarType::TopRight, "open file", "not choose any file!", 2700, this);
        return;
    }
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
    ui->thumbnail_view->reset();
    ui->search_result_view->reset();

    auto status = document->load(file_name);
    if (status == QPdfDocument::Error::None) {
        ElaMessageBar::success(ElaMessageBarType::TopRight,
                               "load file",
                               "successfully load the pdf document!",
                               2700,
                               this);
        // set the page
        page_jump(0);
        int total_page = document->pageCount();
        ui->total_page->setText(QString("/   %1").arg(total_page));
        ui->current_page->setRange(1, total_page);
    } else {
        ElaMessageBar::error(
            ElaMessageBarType::TopRight, "load file", "can not load the pdf document!", 2700, this);
    }
}

void SeaPdf::page_jump(int jump_to, const QPointF& loc) {
    auto nav = ui->pdf_show_page->pageNavigator();
    nav->jump(jump_to, loc, nav->currentZoom());
}


void SeaPdf::enable_thumbnail_checked(bool checked) {
    auto index = ui->switch_tabs->indexOf(ui->thumbnail_container);
    ui->switch_tabs->setTabEnabled(index, checked);
}


void SeaPdf::zoom_slider_changed(int value) {
    double zoom_factor = static_cast<double>(value) / 100.0;
    zoom_factor = this->claim_zoom_factor(zoom_factor);
    // zoom_factor = compute_zoom_factor()
    ui->pdf_show_page->setZoomFactor(zoom_factor);
}


}   // namespace client
}   // namespace tang