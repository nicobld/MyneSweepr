#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <queue>

#include "client.h"
#include "../state/state.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

extern int sockfd;
extern char buf[BUFSIZE];

class State;

Client::Client(State* state) : state(state){
	struct sockaddr_in servaddr;

	if (pthread_mutex_init(&request_mutex, NULL) == -1){
		perror("pthread_mutex_init\n");
		exit(EXIT_FAILURE);
	}
 
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


std::string Client::http_get(const char* uri){
	char* b;
	ssize_t recvsize;
	json j;
	std::string s;

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

	s.assign(token);

	return s;
}


void Client::http_post(const char* command){
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

void Client::addRequest(std::string request){
	pthread_mutex_lock(&request_mutex);
	requests.push(request);
	pthread_mutex_unlock(&request_mutex);
}

void* Client::client_thread(){
	clock_t time_start;
	bool started_clock = false;
	const int trigger = CLIENT_REFRESH_PERIOD;
	const char uri[] = "/state ";

	while(1){
		if (started_clock == false){
			started_clock = true;
			time_start = clock();
		}
		else if (((clock() - time_start) * 1000 / CLOCKS_PER_SEC) >= trigger){
			started_clock = false;
			json js;
			std::string body;

			body = http_get(uri);
			js = json::parse(body);

			state->updateState(js);
		}

		pthread_mutex_lock(&request_mutex);

		if (requests.empty() == false){
			http_post(requests.front().data());

			requests.pop();
		}

		pthread_mutex_unlock(&request_mutex);

		usleep(SLEEP_CLIENT);
	}
}