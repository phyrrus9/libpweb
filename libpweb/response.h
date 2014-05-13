//
//  response.h
//  libpweb
//
//  Created by Ethan Laur on 5/8/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#ifndef __libpweb__response__
#define __libpweb__response__

#include <stdlib.h>
#include <sys/types.h>
#include <vector>

namespace pweb
{
	enum HTTPResponseCode
	{
		kContinue = 100,
		kSwitching_Protocols = 101,
		kOK = 200,
		kCreated = 201,
		kAccepted = 202,
		kNon_Auth_Info = 203,
		kNoContent = 204,
		kResetContent = 205,
		kPartialContent = 206,
		kMultipleChoices = 300,
		kMovedPermanently = 301,
		kFound = 302,
		kSeeOther = 303,
		kNotModified = 304,
		kUseProxy = 305,
		kTemporaryRedirect = 307,
		kBadRequest = 400,
		kUnauthorized = 401,
		kPaymentRequired = 402,
		kForbidden = 403,
		kNotFound = 404,
		kMethodNotAllowed = 405,
		kNotAcceptable = 406,
		kProxyAuthenticationRequired = 407,
		kRequestTimedOut = 408,
		kConflict = 409,
		kGone = 410,
		kLengthRequired = 411,
		kPreconditionFailed = 412,
		kRequestEntityTooLarge = 413,
		kRequestURITooLarge = 414,
		kUnsupportedMediaType = 415,
		kRequestRangeNotSatisfiable = 416,
		kExpectationFailed = 417,
		kInternalServerError = 500,
		kNotImplemented = 501,
		kBadGateway = 502,
		kServiceUnavailable = 503,
		kGatewayTimeout = 504,
		kHTTPVersionNotSupported = 505
	};
#define HTTPRESPONSECODENUM 40

	typedef struct _HTTPResponseStr
	{
		HTTPResponseCode code;
		const char       *str;
	} HTTPResponseStr;
	
	const static HTTPResponseStr HTTPResponses[HTTPRESPONSECODENUM] =
	{
		{ kContinue, "Continue" },
		{ kSwitching_Protocols, "Switching Protocols" },
		{ kOK, "OK" },
		{ kCreated, "Created" },
		{ kAccepted, "Accepted" },
		{ kNon_Auth_Info, "Non Authorative Information" },
		{ kNoContent, "No Content" },
		{ kResetContent, "Reset Content" },
		{ kPartialContent, "Partial Content" },
		{ kMultipleChoices, "Multiple Choices" },
		{ kMovedPermanently, "Moved Permanently" },
		{ kFound, "Found" },
		{ kSeeOther, "See Other" },
		{ kNotModified, "Not Modified" },
		{ kUseProxy, "Use Proxy" },
		{ kTemporaryRedirect, "Temporary Redirect" },
		{ kBadRequest, "Bad Request" },
		{ kUnauthorized, "Unauthorized" },
		{ kPaymentRequired, "Payment Required" },
		{ kForbidden, "Forbidden" },
		{ kNotFound, "Not Found" },
		{ kMethodNotAllowed, "Method Not Allowed" },
		{ kNotAcceptable, "Not Acceptable" },
		{ kProxyAuthenticationRequired, "Proxy Authentication Required" },
		{ kRequestTimedOut, "Request Timed Out" },
		{ kConflict, "Conflict" },
		{ kGone, "Gone" },
		{ kLengthRequired, "Length Required" },
		{ kPreconditionFailed, "Precondition Failed" },
		{ kRequestEntityTooLarge, "Request Entity Too Large" },
		{ kRequestURITooLarge, "Request URI Too Large" },
		{ kUnsupportedMediaType, "Unsupported Media Type" },
		{ kRequestRangeNotSatisfiable, "Request Range Not Satisfiable" },
		{ kExpectationFailed, "Expectation Failed" },
		{ kInternalServerError, "Internal Server Error" },
		{ kNotImplemented, "Not Implemented" },
		{ kBadGateway, "Bad Gateway" },
		{ kServiceUnavailable, "Service Unavailable" },
		{ kGatewayTimeout, "Gateway Timeout" },
		{ kHTTPVersionNotSupported, "HTTP Version Not Supported" }
	};
	
	enum HTTPResponseVersion { kHTTPv1, kHTTPv11 };
	
	typedef struct _HTTPResponseEntity
	{
		char *encoding,
			*language,
			*location,
			*MD5,
			*range,
			*type,
			*expires,
			*last_modified;
		uint32_t length;
	} HTTPResponseEntity;
	
	typedef struct _HTTPResponseHeader
	{
		HTTPResponseCode code;
		HTTPResponseVersion version;
		char *responsemeaning;
		HTTPResponseEntity entity;
	} HTTPResponseHeader;
	
	enum ResponseParam
	{
		kNone =           (1 << 0x00),
		kSentHeader =     (1 << 0x01),
		kCustomModified = (1 << 0x02),
		kCustomResponse = (1 << 0x03),
		kCustomLength =   (1 << 0x04),
		kAllFlags =       (kSentHeader | kCustomModified | kCustomResponse | kCustomLength)
	};
	enum ResponseOpcode
	{
		kResetFlag = 1
	};
	
	class response
	{
	protected:
		HTTPResponseHeader head;
		bool sentHeader,
			customModified,
			customResponse,
			customLength;
		char *getHTTPResponseStr(HTTPResponseCode);
		char *getLastModifiedNow();
		char *getLastModified(time_t);
		unsigned char *genHTTPResponseHeader();
		std::vector<unsigned char> data;
		int sockfd;
	public:
		response(HTTPResponseCode code, int sfd, const char *encoding = NULL, HTTPResponseVersion version = kHTTPv11);
		~response(void);
		void setEncoding(const char *);
		void setLanguage(const char *);
		void setMD5(const char *);
		void setRange(const char *);
		void setType(const char *);
		void setExpires(const char *);
		void setLast_Modified(const char *); //sets customModified flag
		void setResponse(const char *e, HTTPResponseCode code = (HTTPResponseCode)-1); //sets customResponse flag
		void setLength(uint32_t); //sets customLength flag
		void write(void *, ssize_t);
		void sendHeader(void); //should not be used unless you know what you're doing, sets sentHeader flag
		void send(void); //sends all data
		void modifyData(ResponseParam p = kNone, ResponseOpcode o = (ResponseOpcode)kNone);
	};
}

#endif /* defined(__libpweb__response__) */
