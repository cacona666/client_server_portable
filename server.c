#include <stdio.h> 
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <winsock.h>
#else //Linux
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>//any useful in Windows?

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




// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	// infinite loop for chat 
	for (;;) { 
		memset(buff, 0, sizeof(buff));

		// read the message from client and copy it in buffer 
#ifdef WIN32
		n = recv(sockfd, buff, sizeof(buff), 0);
#else
		n = read(sockfd, buff, sizeof(buff)); //POSIX read() not working?
#endif
		// print buffer which contains the client contents 
		printf("From client: %s\n\t To client : ", buff);
		memset(buff, 0, sizeof(buff));
		n = 0; 
		// copy server message in the buffer 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		// and send that buffer to client 
		//Discard EOL
#ifdef WIN32
		send(sockfd, buff, strlen(buff) - 1, 0);
#else
		write(sockfd, buff, strlen(buff) - 1);
#endif
		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("exit", buff, 4) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} 
	} 
} 

// Driver function 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

#ifdef WIN32
    init();
#endif
	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed.... Error: %d, strerror: '%s'\n", errno, strerror(errno));
		exit(0); 
	} 
	else
		printf("Socket successfully created.. (%d)\n", sockfd);
	memset(&servaddr, 0, sizeof(servaddr));

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

    //Reuse port
#ifdef WIN32
    BOOL bOptVal = TRUE;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, sizeof(bOptVal));
#else
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#endif

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed.... Errno: %d, error: '%s'\n", errno, strerror(errno));
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 
	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n");

	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
#ifdef WIN32
    closesocket(sockfd);
#else
	close(sockfd); 
#endif
#ifdef WIN32
    end();
#endif
} 
