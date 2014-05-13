//
//  connectedSocket.h
//  libsocket
//
//  Created by Ethan Laur on 12/30/13.
//  Copyright (c) 2013 Ethan Laur. All rights reserved.
//

#ifndef __libsocket__connectedSocket__
#define __libsocket__connectedSocket__

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

class connectedSocket
{
protected:
	int sockfd;
public:
	connectedSocket(int fd);
	connectedSocket(connectedSocket &old);
	~connectedSocket();
	ssize_t Write(void *data, ssize_t size);
	ssize_t Read(void *data, ssize_t size);
	void Close();
};

#endif /* defined(__libsocket__connectedSocket__) */
