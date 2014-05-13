//
//  response.cpp
//  libpweb
//
//  Created by Ethan Laur on 5/8/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#include "response.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

namespace pweb
{
	static const char lmt_days[][7] =
	{ "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
	
	static const char lmt_months[][12] =
	{
		"January", "Febuary", "March", "April", "May", "June", "July",
		"August", "September", "October", "November", "December"
	};
	
	char *response::getLastModified(time_t t)
	{
		char *str = (char *)malloc(128);
		struct tm *gtm = gmtime(&t);
		memset(str, 0, 128);
		sprintf(str, "%s, %02d %s %04d %02d:%02d:%02d GMT",
			   lmt_days[gtm->tm_wday],
			   gtm->tm_mday,
			   lmt_months[gtm->tm_mon],
			   gtm->tm_year + 1900,
			   gtm->tm_hour,
			   gtm->tm_min,
			   gtm->tm_sec);
		return str;
	}
	
	char *response::getLastModifiedNow()
	{
		time_t t = time(NULL);
		return this->getLastModified(t);
	}
	
	char *response::getHTTPResponseStr(HTTPResponseCode c)
	{
		for (int i = 0; i < HTTPRESPONSECODENUM; i++)
			if (HTTPResponses[i].code == c)
				return (char *)HTTPResponses[i].str;
		return (char *)"Error Fetching Response String";
	}
	
	unsigned char *response::genHTTPResponseHeader()
	{
		//needs implementation
		return NULL;
	}
	
	response::response(HTTPResponseCode code, int sfd, const char *encoding, HTTPResponseVersion version)
	{
		this->head.code = code;
		this->head.responsemeaning = this->getHTTPResponseStr(this->head.code);
		this->head.version = version;
		if (encoding != NULL)
		{
			this->head.entity.encoding = (char *)malloc(strlen(encoding) + 2);
			strcpy(this->head.entity.encoding, encoding);
		}
		this->head.entity.length = 0;
		this->head.entity.last_modified = this->getLastModifiedNow();
		this->sentHeader = false;
		this->customModified = false;
		this->customResponse = false;
		this->customLength = false;
		this->sockfd = sfd;
	}
	
	response::~response()
	{
#define op(a) if (a != NULL) { free(a); }
		op(this->head.entity.encoding);
		op(this->head.entity.expires);
		op(this->head.entity.language);
		op(this->head.entity.last_modified);
		op(this->head.entity.location);
		op(this->head.entity.MD5);
		op(this->head.entity.range);
		op(this->head.entity.type);
#undef  op
	}
	
	void response::setEncoding(const char *e)
	{
		if (this->head.entity.encoding != NULL)
			free(this->head.entity.encoding);
		this->head.entity.encoding = (char *)malloc(strlen(e) + 2);
		strcpy(this->head.entity.encoding, e);
	}
	
	void response::setLanguage(const char *e)
	{
		if (this->head.entity.language != NULL)
			free(this->head.entity.language);
		this->head.entity.language = (char *)malloc(strlen(e) + 2);
		strcpy(this->head.entity.language, e);
	}
	
	void response::setMD5(const char *e)
	{
		if (this->head.entity.MD5 != NULL)
			free(this->head.entity.MD5);
		this->head.entity.MD5 = (char *)malloc(strlen(e) + 2);
		strcpy(this->head.entity.MD5, e);
	}
	
	void response::setRange(const char *e)
	{
		if (this->head.entity.range != NULL)
			free(this->head.entity.range);
		this->head.entity.range = (char *)malloc(strlen(e) + 2);
		strcpy(this->head.entity.range, e);
	}
	
	void response::setType(const char *e)
	{
		if (this->head.entity.type != NULL)
			free(this->head.entity.type);
		this->head.entity.type = (char *)malloc(strlen(e) + 2);
		strcpy(this->head.entity.type, e);
	}
	
	void response::setExpires(const char *e)
	{
		if (this->head.entity.expires != NULL)
			free(this->head.entity.expires);
		this->head.entity.expires = (char *)malloc(strlen(e) + 2);
		strcpy(this->head.entity.expires, e);
	}
	
	void response::setLast_Modified(const char *e)
	{
		if (this->head.entity.last_modified != NULL)
			free(this->head.entity.last_modified);
		this->head.entity.last_modified = (char *)malloc(strlen(e) + 2);
		strcpy(this->head.entity.last_modified, e);
		this->customModified = true;
	}
	
	void response::setResponse(const char *e, HTTPResponseCode code)
	{
		if (this->customResponse)
			if (this->head.responsemeaning != NULL)
				free(this->head.responsemeaning);
		this->head.responsemeaning = (char *)malloc(strlen(e) + 2);
		strcpy(this->head.responsemeaning, e);
		this->customResponse = true;
	}
	
	void response::setLength(uint32_t l)
	{
		this->head.entity.length = l;
		this->customLength = true;
	}
	
	void response::write(void *dp, ssize_t s)
	{
		unsigned char *d = (unsigned char *)dp;
		ssize_t i;
		for (i = 0; i < s; i++)
			this->data.push_back(d[i]);
		if (!this->customLength)
			this->head.entity.length += (size_t)s;
	}
	
	void response::sendHeader()
	{
		unsigned char *header = this->genHTTPResponseHeader();
		::write(this->sockfd, header, strlen((char *)header));
		this->sentHeader = true;
	}
	
	void response::send()
	{
		if (!this->sentHeader)
			this->sendHeader();
		size_t i;
		for (i = 0; i < this->head.entity.length; i++)
			::write(this->sockfd, &this->data[i], 1);
	}
	
	void response::modifyData(ResponseParam p, ResponseOpcode o)
	{
		switch (o)
		{
			case kResetFlag:
				if (p == kNone)
					break;
				if (p & kSentHeader)
					this->sentHeader = false;
				if (p & kCustomModified)
					this->customModified = false;
				if (p & kCustomResponse)
					this->customResponse = false;
				if (p & kCustomLength)
					this->customLength = false;
			default:
				break;
		}
	}
}
