#pragma once

#include "ElaComboBox.h"
#include <QpdfView>
#include <qpdfpageselector.h>

namespace tang {
namespace client {


class ZoomSelector : public ElaComboBox {
    Q_OBJECT
public:
    ZoomSelector(QWidget* parent = nullptr);

public slots:
    void set_zoom_factor(double zoom_factor);

    void reset();

signals:
    void zoom_mode_changed(QPdfView::ZoomMode zoom_mode);
    void zoom_factor_changed(double zoom_factor);

private slots:
    void on_current_text_changed(const QString& text);
};


}   // namespace client
}   // namespace tang
