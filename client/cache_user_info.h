#pragma once
#include <QString>

namespace tang {
namespace client {
class CacheUserInfo{
private:
    QString user_name;
    QString email;
    uint8_t user_id;
    uint8_t vote_prioirty;
public:
    CacheUserInfo();
    void set_user_name(const QString& user_name_);
    QString& get_user_name();

    void set_email(const QString& email_);
    QString& get_email();

    void set_user_id(uint8_t user_id_);
    uint8_t get_user_id();

    void set_vote_priority(uint8_t vote_priority_);
    uint8_t get_vote_priority();
};
}
}