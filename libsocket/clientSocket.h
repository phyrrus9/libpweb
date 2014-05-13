//
//  clientSocket.h
//  libsocket
//
//  Created by Ethan Laur on 12/30/13.
//  Copyright (c) 2013 Ethan Laur. All rights reserved.
//

#ifndef __libsocket__clientSocket__
#define __libsocket__clientSocket__

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

#define CS_NOSOCK  (0x01 << 0x00)
#define CS_PTONERR (0x01 << 0x01)
#define CS_CONFAIL (0x01 << 0x02)
#define CS_OK      (0x01 << 0x03)

class clientSocket
{
protected:
	connectedSocket *sock;
	unsigned char status;
public:
	clientSocket(char *host, int port);
	clientSocket(char *host); //default to port 80
	clientSocket(clientSocket &old);
	~clientSocket();
	unsigned char getStatus(); //returns the flags in status
	ssize_t Write(void *data, ssize_t size);
	ssize_t Read(void *data, ssize_t size);
	void Close();
};

#endif /* defined(__libsocket__clientSocket__) */
