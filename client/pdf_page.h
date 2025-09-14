#include <QWidget>
#include "ElaWidget.h"
#include <QPdfSearchModel>
#include <QPdfDocument>
#include <QFileDialog>
// #include <QLabel>

namespace tang {
namespace client {
class SeaPdfUi;
class SeaPdf:public ElaWidget{
Q_OBJECT
private:
    SeaPdfUi* ui;
    QPdfSearchModel* search_model;
    QPdfDocument* document;
    QString previous_search_text;

    double compute_zoom_factor(bool zoom_in);
    //claim and show some message!
    double claim_zoom_factor(double zoom_factor);
    void init_connects();

    void page_jump(int page_index,const QPointF& loc = {});

public:
    SeaPdf(QWidget* parent=nullptr);

private slots:
    // //selcet the book_mark
    void bookmark_selected(const QModelIndex& index);
    void page_selected(int page);
    void search_result_selected(const QModelIndex& current,const QModelIndex& previous);

    void search_button_clicked();

    // void open_file_clicked();
    void zoom_in_clicked();
    void zoom_out_clicked();

    void previous_page_triggered();
    void next_page_triggered();

    void thumbnail_view_activated(const QModelIndex& index);

    void back_clicked();
    void forward_clicked();

    void find_previous_clicked();
    void find_next_clicked();
    void continous_switch_checked(bool checked);
    void open_file_clicked();

    void enable_thumbnail_checked(bool checked);

    void zoom_slider_changed(int value);

};
}
}