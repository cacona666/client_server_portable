#include <stdio.h> 
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <winsock.h>
#else //Linux
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <errno.h>


#define MAX 80 
#define PORT 8088
#define SA struct sockaddr 




//----------------------------------------------------
//Functions to call in Windows
static void init(void)
{
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

static void end(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}
//----------------------------------------------------



void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	for (;;) { 
		memset(buff, 0, sizeof(buff));
		printf("Enter the string : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
#ifdef WIN32
		//Discard EOL
		send(sockfd, buff, strlen(buff) - 1, 0);//POSIX write() not working?
#else
		write(sockfd, buff, strlen(buff) - 1);
#endif
		memset(buff, 0, sizeof(buff));
#ifdef WIN32
		n = recv(sockfd, buff, sizeof(buff), 0);
#else
		n = read(sockfd, buff, sizeof(buff));
#endif
		printf("From Server : %s\n", buff);
		if ((strncmp(buff, "exit", 4)) == 0) { 
			printf("Client Exit...\n"); 
			break; 
		} 
	} 
} 

int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

#ifdef WIN32
    init();
#endif
	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	memset(&servaddr, 0, sizeof(servaddr));

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	// function for chat 
	func(sockfd); 

	// close the socket 
#ifdef WIN32
    closesocket(sockfd);
#else
	close(sockfd); 
#endif

#ifdef WIN32
    end();
#endif
} 
