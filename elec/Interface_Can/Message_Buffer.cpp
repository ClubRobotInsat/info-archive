//
// Created by paul on 25/08/16.
//

#include "Message_Buffer.h"

Message_Buffer::Message_Buffer(int buffersize) : _buffersize(buffersize), _acceptNewMessage(true) {
    _messageList.clear();

}


bool Message_Buffer::addMessage(const Trame &trame) {
    if (_acceptNewMessage) {
        _messageList.push_front(trame);
         if (_messageList.size() > _buffersize) {
             _messageList.pop_back();
         }
        return true;
    }
    else {
        return false;
    }
}

void Message_Buffer::toogleAcceptNewMessage() {
    _acceptNewMessage = !_acceptNewMessage;

}

Trame Message_Buffer::retrieveMessage() {
    return _messageList.back();
}

Trame Message_Buffer::retrieveMessageMatchingFilter(std::string filter, bool oldest) {
    if (not _messageList.empty()) {
        if (filter != "") {
            if (oldest) {
                return _messageList.back();
            } else {
                return _messageList.front();
            }
        } else {
            return _messageList.front();
        }
    }
    else {
        throw std::runtime_error("Oups, problem");
    }
}
