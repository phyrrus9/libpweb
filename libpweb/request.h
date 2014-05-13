//
//  request.h
//  libpweb
//
//  Created by Ethan Laur on 5/8/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#ifndef __libpweb__request__
#define __libpweb__request__

#include <time.h>
#include <string>
namespace pweb
{
	enum HTTPVersion { kHTTPv1, kHTTPv11 };
	enum HTTPRequest { kOPTIONS, kGET, kHEAD, kPOST, kPUT, kDELETE, kTRACE, kCONNECT };
	
	typedef struct _HTTPHeaderAccept
	{
		char *accept,
			*charset,
			*encoding,
			*language;
	} HTTPHeaderAccept;
	typedef struct _HTTPHeaderHostinfo
	{
		char *authorization,
			*expect,
			*from,
			*host;
	} HTTPHeaderHostinfo;
	typedef struct _HTTPHeaderIf
	{
		char *match,
			*none_match,
			*modified_since,
			*range,
			*unmodified_since;
	}HTTPHeaderIf;
	typedef struct _HTTPHeaderNetinfo
	{
		char *max_forwards,
			*proxy_authorization,
			*range;
	} HTTPHeaderNetinfo;
	typedef struct _HTTPHEaderClientinfo
	{
		char *referer,
			*te,
			*user_agent;
	} HTTPHEaderClientinfo;
	
	typedef struct _HTTPHeader
	{
		HTTPHeaderAccept     accept;
		HTTPHeaderHostinfo   hostinfo;
		HTTPHeaderIf         sectif;
		HTTPHeaderNetinfo    netinfo;
		HTTPHEaderClientinfo clientinfo;
		time_t               recvtime;
	} HTTPHeader;
	
	class request
	{
	protected:
		HTTPVersion protocolversion;
		HTTPRequest requesttype;
		HTTPHeader  header;
		char        URI[200];
		void parseHTTPHeader(const char *); //parses the request into its HTTPHeader fields
	public:
		request(const char *); //takes the entire header request in
		~request(void); //free()'s the entire block (thats a lot of memory, use this)
		/*begin getters*/
		HTTPVersion getProtocolVersion(void);
		HTTPRequest getRequestType(void);
		time_t      getRecvTime(void);
		/*begin HTTPHeaderAccept*/
		const char *getAccept(void);
		const char *getAccept_Charset(void);
		const char *getAccept_Encoding(void);
		const char *getAccept_Language(void);
		/*end HTTPHeaderAccept*/
		/*begin HTTPHeaderHostinfo*/
		const char *getAuthorization(void);
		const char *getExpect(void);
		const char *getFrom(void);
		const char *getHost(void);
		/*end HTTPHeaderHostinfo*/
		/*begin HTTPHeaderIf*/
		const char *getIf_Match(void);
		const char *getIf_Modified_since(void);
		const char *getIf_None_Match(void);
		const char *getIf_Range(void);
		const char *getIf_Unmodified_Since(void);
		/*end HTTPHeaderIf*/
		/*begin HTTPHeaderNetinfo*/
		const char *getMax_Forwards(void);
		const char *getProxy_Authorization(void);
		const char *getRange(void);
		/*end HTTPHeaderNetinfo*/
		/*begin HTTPHeaderclientinfo*/
		const char *getReferrer(void);
		const char *getTE(void);
		const char *getUser_Agent(void);
		/*end HTTPHeaderclientinfo*/
		const char *operator[](const char *); //specify one of the HTTP Fields
		const std::string operator[](std::string); //for compadibility (even though string is fucking stupid)
	};
}

#endif /* defined(__libpweb__request__) */
