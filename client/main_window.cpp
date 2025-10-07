#include "main_window.h"
#include "ElaDockWidget.h"
#include "chat_room.h"
#include "client_singleton.h"
#include "participate_vote_page.h"
#include "util.h"
#include "vote_page.h"
#include "vote_result_page.h"
#include "workspace_page.h"


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
    RemoteWorkspacePage* remote_file_page = new RemoteWorkspacePage(this);
    addPageNode("工作空间", remote_file_page, ElaIconType::CabinetFiling);

    VotePage* vote_page = new VotePage(this);
    addPageNode("发起投票", vote_page, ElaIconType::FaceKiss);

    ParticipateVotePage* participate_vote_page = new ParticipateVotePage(this);
    addPageNode("开始投票", participate_vote_page, ElaIconType::Clouds);

    VoteResultPage* vote_result_page = new VoteResultPage(this);
    addPageNode("投票结果", vote_result_page, ElaIconType::Joystick);

    ElaDockWidget* chat_dock = new ElaDockWidget(QString("聊天室"), this);
    // chat_dock->setDockLocation(Qt::DockWidgetArea::RightDockWidgetArea);
     chat_room = new ChatRoom(this);

    chat_dock->setWidget(chat_room);
    this->addDockWidget(Qt::RightDockWidgetArea, chat_dock);
    this->resizeDocks({chat_dock}, {300}, Qt::Horizontal);
}
}   // namespace client
}   // namespace tang