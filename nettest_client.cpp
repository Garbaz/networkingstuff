#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define QUIT(ret) freeaddrinfo(servinfo); return ret;

using namespace std;

int status, sockfd, msg_len, bytes_sent;
string msg;
struct addrinfo hints;
struct addrinfo *servinfo;

int main(int argc, char* argv[])
{
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if(argc != 2)
	{
		cerr << "{1} Usage: " << argv[0] << " hostname" << endl;
		QUIT(1);
	}
	
	if((status = getaddrinfo(argv[1], "1729", &hints, &servinfo)) != 0)
	{
		cerr << "{2} getaddrinfo error: " << gai_strerror(status) << endl;
		QUIT(2);
	}
	
	if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
	{
		cerr << "{3} socket error: Could not get socket file descriptor" << endl;
		QUIT(3);
	}
	
	
	//NOT required, due to being client
	/*
	if((bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) == -1)
	{
		cerr << "{4} bind error: Unable to bind to port" << endl;
		QUIT(4);
	}
	*/
	
	if(connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		cerr << "{5} connect error: Unable to connect to target host" << endl;
		QUIT(5);
	}
	
	cout << "Successfully connected to server! Type away:" << endl;
	
	while(1)
	{
		cout << ">";
		getline(cin, msg);
		msg += '\n';
		
		msg_len = msg.length();
		
		cout << "Sending message \"" << msg << "\" (" << msg_len << " characters long)" << endl;
		
		do
		{
			if((bytes_sent = send(sockfd, msg.c_str(), msg_len, 0)) == -1)
			{
				cerr << "{6} send error: Unable to send message" << endl;
				QUIT(6);
			}
		}while(bytes_sent < msg_len);
	}
	
	
	freeaddrinfo(servinfo);
	
	return 0;
}
