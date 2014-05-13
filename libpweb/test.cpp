#include <stdio.h>
#include "request.h"

int main()
{
	pweb::request r("GET /hello.hai HTTP/1.1\r\nRand: hello\r\nUser-Agent: meeee!\r\n\r\n");
	printf("%s\n", r["User-Agent"]);
}
