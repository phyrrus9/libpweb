//
//  connectedSocket.cpp
//  libsocket
//
//  Created by Ethan Laur on 12/30/13.
//  Copyright (c) 2013 Ethan Laur. All rights reserved.
//

#include "connectedSocket.h"

/*
 class connectedSocket
 {
 protected:
 int sockfd;
 public:
 connectedSocket(int fd);
 connectedSocket(connectedSocket &old);
 ~connectedSocket();
 int write(void *data, unsigned long size);
 int read(void *data, unsigned long size);
 void close();
 };
 */

connectedSocket::connectedSocket(int fd)
{
	sockfd = fd;
}

connectedSocket::connectedSocket(connectedSocket &old)
{
	sockfd = old.sockfd;
}

connectedSocket::~connectedSocket()
{
	Close();
}

ssize_t connectedSocket::Write(void *data, ssize_t size)
{
	return write(sockfd, data, size);
}

ssize_t connectedSocket::Read(void *data, ssize_t size)
{
	return read(sockfd, data, size);
}

void connectedSocket::Close()
{
	close(sockfd);
}
