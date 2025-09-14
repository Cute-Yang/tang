#include "zoom_selector.h"
#include <ElaComboBox.h>
#include <QLatin1Char>
#include <QLineEdit>
#include <QString>


namespace tang {
namespace client {

ZoomSelector::ZoomSelector(QWidget* parent)
    : ElaComboBox(parent) {
    setEditable(true);

    QStringList zoom_selects = {"适合宽度",
                                "适合整页",
                                "10%",
                                "20%",
                                "50%",
                                "75%",
                                "100%",
                                "125%",
                                "150%",
                                "200%",
                                "250%",
                                "500%"};
    this->addItems(zoom_selects);
    this->setCurrentText(zoom_selects.at(0));
    // auto font = this->font();
    // font.setFamily("微软雅黑");
    

    // init connect!
    connect(this, &ElaComboBox::currentTextChanged, this, &ZoomSelector::on_current_text_changed);

    connect(lineEdit(), &QLineEdit::editingFinished, this, [this]() {
        this->on_current_text_changed(lineEdit()->text());
    });
}

void ZoomSelector::set_zoom_factor(double zoom_factor) {
    // zoom_factor = std::max(std::min(zoom_factor, max_zoom_factor), min_zoom_factor);
    setCurrentText(QString::number(qRound(zoom_factor * 100)) + QLatin1String("%"));
}

void ZoomSelector::reset() {
    setCurrentIndex(6);
}

void ZoomSelector::on_current_text_changed(const QString& text) {
    if (text == QStringLiteral("适合宽度")) {
        emit zoom_mode_changed(QPdfView::ZoomMode::FitToWidth);
    } else if (text == QStringLiteral("适合整页")) {
        emit zoom_mode_changed(QPdfView::ZoomMode::FitInView);
    } else {
        double  factor = 1.0;
        QString remove_percent(text);
        remove_percent.remove(QLatin1Char('%'));
        bool ok         = false;
        int  zoom_level = remove_percent.toInt(&ok);
        if (ok) {
            factor = zoom_level / 100.0;
        }
        emit zoom_mode_changed(QPdfView::ZoomMode::Custom);
        emit zoom_factor_changed(factor);
    }
}



}   // namespace client
}   // namespace tang