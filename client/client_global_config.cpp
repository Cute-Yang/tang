#include "client_global_config.h"

namespace tang {
namespace client {
//first arg is object name,second arg is icon file path
const QString ClientGlobalConfig::button_style_sheet_fmt = R"(
        QToolButton#%1{
            background-color:#20B2AA;
            font-size: 12px;
            font-weight: normal;
            border: 2px solid #b9b9b9; 
            border-radius: 3px;
            qproperty-icon: url("%2") center;
            qproperty-iconSize: 16px 16px;
            padding-left: 15px;
        }
    )";
}
}   // namespace tang