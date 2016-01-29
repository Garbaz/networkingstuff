#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORT "1729"
#define BACKLOG 10
#define BUFFER_LEN 32
#define QUIT(ret) cleanup(); return ret;

using namespace std;

void cleanup();

int status, sockfd, newfd, msg_len, bytes_recieved;
string msg;
char* buffer;
struct addrinfo hints, *servinfo;;
struct sockaddr_storage conn_addr;
socklen_t conn_addr_size;

int main(int argc, char* argv[])
{
	msg = "PONG!";
	buffer = (char*)malloc(BUFFER_LEN);
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
	{
		cerr << "{2} getaddrinfo error: " << gai_strerror(status) << endl;
		QUIT(2);
	}
	
	if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
	{
		cerr << "{3} socket error: Could not get socket file descriptor" << endl;
		QUIT(3);
	}
	
	if(bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		cerr << "{4} bind error: Unable to bind to port; Try again in a minute" << endl;
		QUIT(4);
	}

	while(1)
	{
		cout << "Waiting for client connection..." << endl;
		if(listen(sockfd, BACKLOG) == -1)
		{
			cerr << "{5} listen error: Unable to listen" << endl;
			QUIT(5);
		}
		
		conn_addr_size = sizeof conn_addr;
		if((newfd = accept(sockfd, (struct sockaddr*)&conn_addr, &conn_addr_size)) == -1)
		{
			cerr << "{6} accept error: Unable to accept incoming connection" << endl;
			QUIT(6);
		}
		
		cout << "Client successfully connected! Here's what he got to say:" << endl;
		
		do
		{
			if((bytes_recieved = recv(newfd, buffer, BUFFER_LEN, 0)) == -1)
			{
				cerr << "{7} recieve error: Something went wrong while recieving message" << endl;
				QUIT(7);
			}
			
			for(int i = 0; i < bytes_recieved; i++)
			{
				putchar(buffer[i]);
			}
		}while(bytes_recieved != 0);
	}
	
	cleanup();
	
	return 0;
}

void cleanup()
{
	free(buffer);
	freeaddrinfo(servinfo);
}
