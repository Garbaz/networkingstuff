#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

using namespace std;

int main(int argc, char* argv[])
{
	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];
	
	if(argc != 2)
	{
		cerr << "Usage: " << argv[0] << " hostname" << endl;
	}
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
	{
		cerr << "getaddrinfo error: " << gai_strerror(status) << endl;
		return 1;
	}
	
	cout << "IP adresses for " << argv[1] << ':' << endl;
	
	for(p = res; p != NULL; p = p->ai_next)
	{
		void* addr;
		string ipver;
		
		if(p->ai_family == AF_INET)
		{
			struct sockaddr_in* ipv4 = (struct sockaddr_in*) p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}
		else
		{
			struct sockaddr_in6* ipv6 = (struct sockaddr_in6*) p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		cout << ipver << ": " << ipstr << endl;
	}
	
	freeaddrinfo(res);
	
	return 0;
}
