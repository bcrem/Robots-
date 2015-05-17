#include <iostream>
#include <unistd.h>

#include <cstdlib>
#include <cstring>

#include <stdio.h>
#include <strings.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>

using namespace std;


#define MY_PORT		9998
#define MAXBUF		1024

int main()
{
	int sockfd, clientfd;
	struct sockaddr_in self;
	char buffer[MAXBUF];

	// Create streaming socket
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket");
		return errno;
	}
	
	/*---Initialize address/port structure---*/
	bzero(&self, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(MY_PORT);
	self.sin_addr.s_addr = INADDR_ANY;

	/*---Assign a port number to the socket---*/
    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 )
	{
		perror("socket--bind");
		return errno;
	}

	/*---Make it a "listening socket"---*/
	if ( listen(sockfd, 20) != 0 )
	{
		perror("socket--listen");
		return errno;
	}

	while (1)
    {
		struct sockaddr_in client;
		int addrlen = sizeof(client);
		
		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client, (socklen_t*)&addrlen);
		printf("%s:%d connected\n",
		        inet_ntoa(client.sin_addr),
		        ntohs(client.sin_port));
		
		int bytesReceived = recv(clientfd, buffer, MAXBUF, 0);
	//	printf ("Received %lu bytes.\n", (unsigned long int) bytesReceived);
	
		// have something
		if (bytesReceived) {
			// assuming input is just a string consisting of a 2 digit integer
            //int val = atoi(buffer);
			//int a = val / 10;				// first digit
			//int b = val % 10;				// second digit
            cout << "Read " << bytesReceived << " bytes." << endl;
            buffer[bytesReceived] = '\0';
			
            cout << "buffer: " << buffer << endl;
		}
		const char *msg = "success";
		send(clientfd,msg,strlen(msg),0);
	}
	
    close(clientfd);

	return 0;
}
