//
//  clientSocket.cpp
//  libsocket
//
//  Created by Ethan Laur on 12/30/13.
//  Copyright (c) 2013 Ethan Laur. All rights reserved.
//

#include "clientSocket.h"

/*
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
 */

clientSocket::clientSocket(char * host, int port)
{
	int sockfd;
	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		status = CS_NOSOCK;
	else if(inet_pton(AF_INET, host, &serv_addr.sin_addr)<=0)
		status = CS_PTONERR;
	else if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		status = CS_CONFAIL;
	else
		status = CS_OK;
	sock = new connectedSocket(sockfd);
}

clientSocket::clientSocket(char *host)
{
	clientSocket::clientSocket(host, 80);
}

clientSocket::clientSocket(clientSocket &old)
{
	if (sock != NULL)
		delete sock;
	sock = new connectedSocket(*old.sock);
}

clientSocket::~clientSocket()
{
	Close();
}

unsigned char clientSocket::getStatus()
{
	return status;
}

ssize_t clientSocket::Write(void *data, ssize_t size)
{
	return sock->Write(data, size);
}

ssize_t clientSocket::Read(void *data, ssize_t size)
{
	return sock->Read(data, size);
}

void clientSocket::Close()
{
	sock->Close();
	delete sock;
}
