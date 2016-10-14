//
// Created by paul on 25/08/16.
//

#include "Header.h"
#include <Trame.h>

#ifndef ROOT_MESSAGE_BUFFER_H
#define ROOT_MESSAGE_BUFFER_H


class Message_Buffer {

public:
	Message_Buffer(int buffersize);

	bool addMessage(const Trame& trame);

	void toogleAcceptNewMessage();

	Trame retrieveMessage(bool oldest);

	bool getAcceptNewMessage();

private:
	std::deque<Trame> _messageList;

	int _buffersize;

	bool _acceptNewMessage;
};


#endif // ROOT_MESSAGE_BUFFER_H
