#pragma once
#include "ElaLineEdit.h"
#include "ElaListView.h"
#include "ElaSlider.h"
#include "ElaSpinBox.h"
#include "ElaTabWidget.h"
#include "ElaToggleSwitch.h"
#include "ElaToolBar.h"
#include "ElaToolButton.h"
#include "ElaTreeView.h"
#include "zoom_selector.h"
#include <ElaText.h>
#include <QSplitter>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <QtPdfWidgets/QPdfView>
namespace tang {
namespace client {

class DisplayPdfUi {
    // declare all widget
public:
    DisplayPdfUi() = default;
    ElaToolBar*    tool_bar;
    ElaToolButton* hide_left_button;
    ElaToolButton* zoom_in_button;
    ZoomSelector*  zoom_selector;
    ElaToolButton* zoom_out_button;
    ElaToolButton* back_history_button;
    ElaSlider*     zoom_slider;
    // start from 1,for example,if a file has 4 page,our range is [1,5)
    ElaSpinBox*    current_page;
    ElaText*       total_page;
    ElaToolButton* forward_history_button;
    ElaToolButton* open_file_button;

    ElaLineEdit*   search_line_edit;
    ElaToolButton* search_button;

    ElaText*         conitnous_text;
    ElaToggleSwitch* continous_switch;

    ElaTabWidget* switch_tabs;
    ElaTreeView*  bookmark_view;

    QWidget*         thumbnail_container;
    QListView*       thumbnail_view;
    ElaListView*     search_result_view;
    ElaToggleSwitch* enable_thumbnail;
    QWidget*         search_result_container;
    ElaText*         enable_thumbnail_text;
    QPdfView*        pdf_show_page;

    // for adjust width
    QWidget* tab_container;
    void     setup_ui(QWidget* window);
};

}   // namespace client
}   // namespace tang