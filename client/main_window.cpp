#include "main_window.h"
#include "ElaContentDialog.h"
#include "ElaDockWidget.h"
#include "chat_room.h"
#include "client_singleton.h"
#include "common/http_json_keys.h"
#include "participate_vote_page.h"
#include "setting_page.h"
#include "util.h"
#include "vote_page.h"
#include "vote_result_page.h"
#include "workspace_page.h"
#include <QUrlQuery>



using namespace tang::common;
namespace tang {
namespace client {

ClientMainWindow::ClientMainWindow(QWidget* parent)
    : ElaWindow(parent) {
    this->init_page();
    this->setFocusPolicy(Qt::StrongFocus);
    this->resize(QSize(1200, 600));
    this->moveToCenter();
    this->setWindowTitle("Butterfly");
    // left info

    // setUserInfoCardVisible(false);
    // 设置窗口标题
}
void ClientMainWindow::init_user_display_info() {
    // setUserInfoCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg"));
    auto& current_user_info = ClientSingleton::get_cache_user_info_instance();
    this->setUserInfoCardTitle(QString("%1 (%2)")
                                   .arg(current_user_info.user_name)
                                   .arg(get_vote_priority_display_str(static_cast<VotePriority>(
                                       current_user_info.vote_prioirty))));

    this->setUserInfoCardSubTitle(current_user_info.email);
}

void ClientMainWindow::init_page() {
    close_dialog = new ElaContentDialog(this);
    connect(close_dialog, &ElaContentDialog::rightButtonClicked, this, [this]() {
        // then set offline!
        auto      url       = ClientSingleton::get_http_urls_instance().get_logout_url();
        auto&     user_info = ClientSingleton::get_cache_user_info_instance();
        QUrlQuery query;
        query.addQueryItem("user_id", QString::number(user_info.user_id));
        auto reply = send_http_req_with_form_data(query, url);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            auto json_doc = get_json_document(reply);
            if (!json_doc) {
                return;
            }
            auto& json_data = json_doc.value();
            // VALIDATE_JSON_RESP_IS_OK(json_data);
            if (json_data[PublicResponseJsonKeys::status_key].toInt() ==
                static_cast<uint8_t>(StatusCode::kSuccess)) {
                this->closeWindow();
            }
        });
    });
    connect(close_dialog, &ElaContentDialog::middleButtonClicked, this, [this]() {
        close_dialog->close();
        showMinimized();
        this->showMinimized();
    });
    this->setIsDefaultClosed(false);

    connect(this, &ClientMainWindow::closeButtonClicked, this, [=]() { close_dialog->exec(); });
    remote_workspace_page = new RemoteWorkspacePage(this);
    addPageNode("工作空间", remote_workspace_page, ElaIconType::ChessQueen);

    vote_page = new VotePage(this);
    addPageNode("发起投票", vote_page, ElaIconType::CommentSmile);

    participate_vote_page = new ParticipateVotePage(this);
    addPageNode("开始投票", participate_vote_page, ElaIconType::Crow);

    vote_result_page = new VoteResultPage(this);
    addPageNode("投票结果", vote_result_page, ElaIconType::ChartMixed);

    // chat page!
    ElaDockWidget* chat_dock = new ElaDockWidget(QString("聊天室"), this);
    chat_room                = new ChatRoom(this);
    chat_dock->setWidget(chat_room);
    this->addDockWidget(Qt::RightDockWidgetArea, chat_dock);
    this->resizeDocks({chat_dock}, {132}, Qt::Horizontal);


    // setting page!
    T_Setting* setting_page = new T_Setting(this);
    addFooterNode("设置", setting_page, setting_key, 0, ElaIconType::GearComplex);
}
}   // namespace client
}   // namespace tang