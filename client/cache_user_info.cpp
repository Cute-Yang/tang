#include "cache_user_info.h"

namespace tang {
namespace client {

CacheUserInfo::CacheUserInfo() = default;

void CacheUserInfo::set_user_name(const QString& user_name_) {
    user_name = user_name_;
}

QString& CacheUserInfo::get_user_name() {
    return user_name;
}

void CacheUserInfo::set_email(const QString& email_) {
    email = email_;
}

QString& CacheUserInfo::get_email() {
    return email;
}


void CacheUserInfo::set_user_id(uint8_t user_id_) {
    user_id = user_id_;
}

uint8_t CacheUserInfo::get_user_id() {
    return user_id;
}

void CacheUserInfo::set_vote_priority(uint8_t vote_priority_) {
    vote_prioirty = vote_priority_;
}

uint8_t CacheUserInfo::get_vote_priority() {
    return vote_prioirty;
}

}   // namespace client
}   // namespace tang