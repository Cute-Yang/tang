#pragma once

#include "ElaScrollPage.h"
#include <QStackedWidget>
#include "workspace_page_ui.h"


namespace tang {
namespace client {

class RemoteWorkspacePage : public ElaScrollPage {
    Q_OBJECT
private:
    RemoteWorkspacePageUi* ui;
    void set_test_model();

public:
    RemoteWorkspacePage(QWidget* parent = nullptr);

    // void setup_ui();
};

}   // namespace client
}   // namespace tang