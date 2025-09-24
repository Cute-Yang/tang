#include "ElaWidget.h"
#include "display_pdf_ui.h"
#include <QFileDialog>
#include <QPdfDocument>
#include <QPdfSearchModel>
#include <QWidget>


namespace tang {
namespace client {
class DisplayPdf : public ElaWidget {
    Q_OBJECT
private:
    DisplayPdfUi*    ui;
    QPdfSearchModel* search_model;
    QPdfDocument*    document;
    QString          previous_search_text;

    double compute_zoom_factor(bool zoom_in);
    // claim and show some message!
    double claim_zoom_factor(double zoom_factor);
    void   init_connects();
    void page_jump(int page_index, const QPointF& loc = {});

    void show_message(const QString& message,bool error=true);

public:
    DisplayPdf(QWidget* parent = nullptr);
    ~DisplayPdf();

private slots:
    // //selcet the book_mark
    void bookmark_selected(const QModelIndex& index);
    void page_selected(int page);
    void search_result_selected(const QModelIndex& current, const QModelIndex& previous);

    void on_search_button_clicked();

    // void open_file_clicked();
    void on_zoom_in_button_clicked();
    void on_zoom_out_button_clicked();

    void on_previous_page_triggered();
    void on_next_page_triggered();

    void on_thumbnail_view_activated(const QModelIndex& index);
    void on_back_button_clicked();
    void on_forward_button_clicked();

    void on_find_previous_button_clicked();
    void on_find_next_button_clicked();
    void on_continous_switch_checked(bool checked);
    void on_open_file_button_clicked();

    void on_enable_thumbnail_checked(bool checked);
    void on_zoom_slider_changed(int value);
};
}   // namespace client
}   // namespace tang