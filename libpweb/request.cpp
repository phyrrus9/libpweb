//
//  request.cpp
//  libpweb
//
//  Created by Ethan Laur on 5/8/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#include "request.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace pweb
{
	char *gethttpline(const char *head, int line)
	{
		int i;
		char *buf = (char *)malloc(1024), *tmp = (char *)head, *tmp2 = NULL;
		for (i = 0; i < line; i++)
		{
			if ((tmp = strstr(tmp, "\r\n")) == NULL)
				goto fail;
			tmp += 2;
			if ((tmp2 = strstr(tmp, "\r\n")) == tmp)
				goto fail; //END OF HEADER { '\r', '\n', /*note, no space*/ '\r', '\n' }
		}
		if ((tmp2 = strstr(tmp, "\r\n")) == NULL)
			goto fail;
		memcpy(buf, tmp, tmp2 - tmp);
		buf[tmp2 - tmp] = 0;
		return buf;
	fail:
		free(buf);
		return NULL;
	}
	
	char *getfield(const char *head, const char *field)
	{
		char *linebuf = NULL; //buffer for reading every line
		char buf[64];
		int i;
		sprintf(buf, "%s: ", field);
		for (i = 0;; i++)
		{
			if ((linebuf = gethttpline(head, i)) == NULL)
				return NULL;
			if (strstr(linebuf, buf) == linebuf)
				return linebuf;
			free(linebuf);
		}
		return NULL;
	}
	
	void request::parseHTTPHeader(const char *headln)
	{
#define op(a, b) \
if ((tmp = getfield(headln, b)) != NULL)\
{\
	a = (char *)malloc(strlen(tmp) + 1);\
	strcpy(a, tmp + strlen(b) + 2);\
	free(tmp);\
}
		char *tmp;
		this->header.recvtime = time(NULL);
		op(this->header.accept.accept, "Accept");
		op(this->header.accept.charset, "Accept-Charset");
		op(this->header.accept.encoding, "Accept-Encoding");
		op(this->header.accept.language, "Accept-Language");
		op(this->header.clientinfo.referer, "Referer");
		op(this->header.clientinfo.te, "TE");
		op(this->header.clientinfo.user_agent, "User-Agent");
		op(this->header.hostinfo.authorization, "Authorization");
		op(this->header.hostinfo.expect, "Expect");
		op(this->header.hostinfo.from, "From");
		op(this->header.hostinfo.host, "Host");
		op(this->header.netinfo.max_forwards, "Max-Forwards");
		op(this->header.netinfo.proxy_authorization, "Proxy-Authorization");
		op(this->header.netinfo.range, "Range");
		op(this->header.sectif.match, "If-Match");
		op(this->header.sectif.modified_since, "If-Modified-Since");
		op(this->header.sectif.none_match, "If-None-Match");
		op(this->header.sectif.range, "If-Range");
		op(this->header.sectif.unmodified_since, "If-Unmodified-Since");
#undef  op
	}
	
	request::request(const char *headln)
	{
		char type[10];
#define op(a) a = NULL
		op(this->header.accept.accept);
		op(this->header.accept.charset);
		op(this->header.accept.encoding);
		op(this->header.accept.language);
		op(this->header.clientinfo.referer);
		op(this->header.clientinfo.te);
		op(this->header.clientinfo.user_agent);
		op(this->header.hostinfo.authorization);
		op(this->header.hostinfo.expect);
		op(this->header.hostinfo.from);
		op(this->header.hostinfo.host);
		op(this->header.netinfo.max_forwards);
		op(this->header.netinfo.proxy_authorization);
		op(this->header.netinfo.range);
		op(this->header.sectif.match);
		op(this->header.sectif.modified_since);
		op(this->header.sectif.none_match);
		op(this->header.sectif.range);
		op(this->header.sectif.unmodified_since);
#undef  op
		sscanf(headln, "%s %s HTTP/1.%d", type, this->URI, &this->protocolversion);
#define op(a) (strcmp(type, a) == 0)
		if (op("OPTIONS"))
			this->requesttype = kOPTIONS;
		if (op("GET"))
			this->requesttype = kGET;
		if (op("HEAD"))
			this->requesttype = kHEAD;
		if (op("POST"))
			this->requesttype = kPOST;
		if (op("PUT"))
			this->requesttype = kPUT;
		if (op("DELETE"))
			this->requesttype = kDELETE;
		if (op("TRACE"))
			this->requesttype = kTRACE;
		if (op("CONNECT"))
			this->requesttype = kCONNECT;
#undef  op
		this->parseHTTPHeader(headln);
	}
	request::~request()
	{
		free(this->header.accept.accept);
		free(this->header.accept.charset);
		free(this->header.accept.encoding);
		free(this->header.accept.language);
		free(this->header.clientinfo.referer);
		free(this->header.clientinfo.te);
		free(this->header.clientinfo.user_agent);
		free(this->header.hostinfo.authorization);
		free(this->header.hostinfo.expect);
		free(this->header.hostinfo.from);
		free(this->header.hostinfo.host);
		free(this->header.netinfo.max_forwards);
		free(this->header.netinfo.proxy_authorization);
		free(this->header.netinfo.range);
		free(this->header.sectif.match);
		free(this->header.sectif.modified_since);
		free(this->header.sectif.none_match);
		free(this->header.sectif.range);
		free(this->header.sectif.unmodified_since);
	}
	
	HTTPVersion request::getProtocolVersion(void)
	{
		return protocolversion;
	}
	HTTPRequest request::getRequestType(void)
	{
		return requesttype;
	}
	time_t request::getRecvTime()
	{
		return header.recvtime;
	}
	
	//HTTPHeaderAccept
	const char *request::getAccept(void)
	{
		return (const char *)header.accept.accept;
	}
	const char *request::getAccept_Charset(void)
	{
		return (const char *)header.accept.charset;
	}
	const char *request::getAccept_Encoding(void)
	{
		return (const char *)header.accept.encoding;
	}
	const char *request::getAccept_Language(void)
	{
		return (const char *)header.accept.language;
	}
	//HTTPHeaderHostinfo
	const char *request::getAuthorization(void)
	{
		return (const char *)header.hostinfo.authorization;
	}
	const char *request::getExpect(void)
	{
		return (const char *)header.hostinfo.expect;
	}
	const char *request::getFrom(void)
	{
		return (const char *)header.hostinfo.from;
	}
	const char *request::getHost(void)
	{
		return (const char *)header.hostinfo.host;
	}
	//HTTPHeaderIf
	const char *request::getIf_Match(void)
	{
		return (const char *)header.sectif.match;
	}
	const char *request::getIf_Modified_since(void)
	{
		return (const char *)header.sectif.modified_since;
	}
	const char *request::getIf_None_Match(void)
	{
		return (const char *)header.sectif.none_match;
	}
	const char *request::getIf_Range(void)
	{
		return (const char *)header.sectif.range;
	}
	const char *request::getIf_Unmodified_Since(void)
	{
		return (const char *)header.sectif.unmodified_since;
	}
	//HTTPHeaderNetinfo
	const char *request::getMax_Forwards(void)
	{
		return (const char *)header.netinfo.max_forwards;
	}
	const char *request::getProxy_Authorization(void)
	{
		return (const char *)header.netinfo.proxy_authorization;
	}
	const char *request::getRange(void)
	{
		return (const char *)header.netinfo.range;
	}
	//HTTPHeaderClientinfo
	const char *request::getReferrer(void)
	{
		return (const char *)header.clientinfo.referer;
	}
	const char *request::getTE(void)
	{
		return (const char *)header.clientinfo.te;
	}
	const char *request::getUser_Agent(void)
	{
		return (const char *)header.clientinfo.user_agent;
	}
	const char *request::operator[](const char * val)
	{
#define op(a) (strcmp(val, a) == 0)
		if (op("Accept"))              return getAccept();
		else if (op("Accept-Charset"))      return getAccept_Charset();
		else if (op("Accept-Encoding"))     return getAccept_Encoding();
		else if (op("Accept-Language"))     return getAccept_Language();
		else if (op("Authorization"))       return getAuthorization();
		else if (op("Expect"))              return getExpect();
		else if (op("From"))                return getFrom();
		else if (op("If-Match"))            return getIf_Match();
		else if (op("If-Modified-Since"))   return getIf_Modified_since();
		else if (op("If-None-Match"))       return getIf_None_Match();
		else if (op("If-Range"))            return getIf_Range();
		else if (op("If-Unmodified-Since")) return getIf_Unmodified_Since();
		else if (op("Max-Forwards"))        return getMax_Forwards();
		else if (op("Proxy-Authorization")) return getProxy_Authorization();
		else if (op("Range"))               return getRange();
		else if (op("Referer"))             return getReferrer();
		else if (op("TE"))                  return getTE();
		else if (op("User-Agent"))          return getUser_Agent();
		else if (op("URI"))                 return this->URI;
		return NULL;
#undef op
	}
	const std::string request::operator[](std::string a)
	{
		return this->operator[](a.c_str()); //yeah...string is just char's bitch
	}
}
