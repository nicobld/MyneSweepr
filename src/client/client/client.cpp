#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

extern int sockfd;
extern char buf[BUFSIZE];

void connect_to_server(){
	struct sockaddr_in servaddr;
 
	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1){
		printf("socket creation failed...\n");
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
 
	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
 
	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0){
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");
}


char* http_get(const char* uri){
	char* b;
	ssize_t recvsize;
	json j;

	memset(buf, 0, BUFSIZE);

	b = buf;

	b += sprintf(b, "GET ");
	b += sprintf(b, "%s", uri);
	b += sprintf(b, "HTTP/1.1\r\n");
	b += sprintf(b, "\r\n");

	if (send(sockfd, buf, strlen(buf), 0) == -1){
		perror("send");
		exit(EXIT_FAILURE);
	}

	memset(buf, 0, BUFSIZE);

	if ((recvsize = recv(sockfd, buf, BUFSIZE, 0)) == -1){
		perror("recv");
		exit(EXIT_FAILURE);
	}
	
	buf[recvsize] = '\0';

	char* saveptr;
	char* token;
	token = strtok_r(buf, " ", &saveptr);
	if (strcmp(token, "HTTP/1.1") != 0){
		printf("http error\n");
		return NULL;
	}
	token = strtok_r(NULL, " ", &saveptr);
	if (*token != '2'){
		printf("http error code: %s\n", token);
		return NULL;
	}

	token = strtok_r(NULL, "\n", &saveptr);
	token = strtok_r(NULL, "\n", &saveptr);
	token += strlen(token) + 1;

	return token;
}


void http_post(char* command){
	char* b = buf;

	memset(buf, 0, BUFSIZE);
	
	b += sprintf(b, "POST ");
	b += sprintf(b, "/ ");
	b += sprintf(b, "HTTP/1.1\r\n");
	b += sprintf(b, "\r\n");

	b += sprintf(b, "%s", command);

	*b = '\0';
	send(sockfd, buf, strlen(buf) + 1, 0);
}