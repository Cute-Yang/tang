#pragma once

// this header define the chat data between sever and client!
namespace tang {
namespace common {
enum class ChatMessageKind {
    // only notify other some one create a new vote!
    kCreateNewVoteAndNotifyAll = 0,
    // notify and send the vote item to other people!
    kCreateNewVote = 1,
    // the creator remove the vote!
    kRemoveVoteAndNotifyAll = 2,
    // and set disabled to client this is invalid...
    kRemoveVote                 = 3,
    kVoteIsFinishedAndNotifyAll = 4,
    kVoteIsFinished             = 5,
    kFriendIsOnline             = 7,
    kFriendIsOffline            = 8,
};


}
}   // namespace tang