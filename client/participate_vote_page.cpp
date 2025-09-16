#include "participate_vote_page.h"

namespace tang {
namespace client {
// use unique_ptr to instead!
ParticipateVotePage::ParticipateVotePage(QWidget* parent)
    : ElaScrollPage(parent)
    , ui(new ParticipateVotePageUi()) {
    ui->setup_ui(this);
    this->setTitleVisible(false);

    connect(ui->confirm_vote_button,&ElaToolButton::clicked,this,[this](){
        ui->vote_status_value->setText("已投票");
        ui->combox_stacked_container->setCurrentIndex(1);
    });
}
ParticipateVotePage::~ParticipateVotePage() {
    delete ui;
}
}   // namespace client
}   // namespace tang