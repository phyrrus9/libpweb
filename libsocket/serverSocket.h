//
//  serverSocket.h
//  libsocket
//
//  Created by Ethan Laur on 12/30/13.
//  Copyright (c) 2013 Ethan Laur. All rights reserved.
//

#ifndef __libsocket__serverSocket__
#define __libsocket__serverSocket__

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
#include "connectedSocket.h"

class serverSocket
{
protected:
	int listenfd;
	struct sockaddr_in serv_addr;
public:
	serverSocket(int port);
	serverSocket(serverSocket &old);
	~serverSocket();
	connectedSocket * Accept();
	void Close();
};

#endif /* defined(__libsocket__serverSocket__) */
