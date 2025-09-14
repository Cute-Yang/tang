#include "search_result_delegate.h"
#include <QFontMetrics>
#include <QPainter>
#include <QPdfSearchModel>
#include <QString>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qstyleditemdelegate.h>


namespace tang {
namespace client {


SearchResultDelegate::SearchResultDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

void SearchResultDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                                 const QModelIndex& index) const {
    const QString display_text = index.data().toString();
    constexpr QStringView left_mark(u"<b>");
    constexpr size_t  left_mark_len = left_mark.length();
    constexpr QStringView right_mark = u"</b>";
    constexpr size_t right_mark_len = right_mark.length();

    size_t left_mark_pos = display_text.indexOf(left_mark,0,Qt::CaseInsensitive);
    size_t right_mark_pos = display_text.indexOf(right_mark,0,Qt::CaseInsensitive);
    
    //means find it!
    if(left_mark_pos >=0 && right_mark_pos > left_mark_pos){
        int page = index.data(static_cast<int>(QPdfSearchModel::Role::Page)).toInt();
        QString page_label = tr("page:%1 ").arg(page);
        size_t pos = left_mark_pos + left_mark_len;
        size_t len = right_mark_pos - pos;
        QString bold_text = display_text.mid(pos,len);
        if(option.state & QStyle::State_Selected){
            painter->fillRect(option.rect,option.palette.highlight());
        }
        QFontMetrics font_metrics = painter->fontMetrics();
        int page_label_width = font_metrics.horizontalAdvance(page_label);
        int y_offset = (option.rect.height() - font_metrics.height()) / font_metrics.ascent();
        int draw_x = 0;
        int draw_y = option.rect.y() + y_offset;

        painter->drawText(draw_x,draw_y,page_label);
        draw_x += page_label_width;

        //for prefix
        QString prefix_text = display_text.left(left_mark_pos);
        int prefix_text_width = font_metrics.horizontalAdvance(prefix_text);
        painter->drawText(draw_x,draw_y,prefix_text);
        draw_x += prefix_text_width;
        //bold
        
        QFont original_font = painter->font();
        auto bold_font = original_font;
        bold_font.setBold(true);
        painter->setFont(bold_font);
        int bold_text_width = font_metrics.horizontalAdvance(bold_text);
        painter->drawText(draw_x,draw_y,bold_text);
        draw_x += bold_text_width;
        painter->setFont(original_font);

        QString suffix_text = display_text.mid(right_mark_pos + right_mark_len);
        painter->drawText(draw_x,draw_y,suffix_text);
    }else{
        QStyledItemDelegate::paint(painter, option, index);
    }
  
}
}   // namespace client
}   // namespace tang