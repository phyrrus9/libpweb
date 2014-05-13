//
//  serverSocket.cpp
//  libsocket
//
//  Created by Ethan Laur on 12/30/13.
//  Copyright (c) 2013 Ethan Laur. All rights reserved.
//

#include "serverSocket.h"

/*
 class serverSocket
 {
 protected:
 int listenfd;
 struct sockaddr_in serv_addr;
 public:
 serverSocket(int port);
 serverSocket(serverSocket &old);
 ~serverSocket();
 connectedSocket Accept();
 void Close();
 };
 */

serverSocket::serverSocket(int port)
{
	memset(&serv_addr, '0', sizeof(struct sockaddr_in));
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
	listen(listenfd, 10);
}

serverSocket::serverSocket(serverSocket &old)
{
	listenfd = old.listenfd;
	serv_addr = old.serv_addr;
}

serverSocket::~serverSocket()
{
	Close();
}

connectedSocket * serverSocket::Accept()
{
	int sockfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
	return new connectedSocket(sockfd);
}

void serverSocket::Close()
{
	close(listenfd);
}
