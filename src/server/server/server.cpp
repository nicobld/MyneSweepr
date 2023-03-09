#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <pthread.h>

#include "server.h"
#include "../engine/engine.h"
#include "../state/state.h"
#include "../common.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Engine;
class State;

extern Engine engine;
extern State state;


typedef struct client_info {
	bool used = false;
	pthread_t thread_id;
	int clifd;
} client_info_t;


static bool handle_GET(char* const buf){
	char* b = buf;
	char* token;
	char uri[BUFSIZE];
	char* saveptr;
	json j;

	if ((token = strtok_r(buf + 4, " ", &saveptr)) == NULL){
		return false;
	}

	strncpy(uri, token, BUFSIZE - 4);

	if (strcmp(uri, "/state") == 0){
		j = state.serializeJSON();
	} else {
		b += sprintf(b, "HTTP/1.1 404 Not Found\r\n");
		b += sprintf(b, "\r\n");
		b += sprintf(b, "\r\n");
		*b = '\0';
		return true;
	}
	b += sprintf(b, "HTTP/1.1 200 OK\r\n");
	b += sprintf(b, "\r\n");
	std::string cpp_str = j.dump();
	const char* s = cpp_str.c_str();
	b += sprintf(b, "%s", s);
	*b = '\0';
	return true;
}


static bool handle_POST(char* const buf){
	char* body;

	if ((body = strstr(buf, "\r\n\r\n")) == NULL){
		return false;
	}
	body += 4;
	std::string s(body);

	engine.pushCommand(s);
	return true;
}


static void* handle_client(void* arg){
	char buf[BUFSIZE];
	ssize_t recvsize;

	client_info_t* client_info = (client_info_t*) arg;

	while(1){
		memset(buf, 0, BUFSIZE);
		recvsize = recv(client_info->clifd, buf, BUFSIZE, 0);
		if (recvsize == -1){
			perror("recv");
			break;
		} else if (recvsize == 0){
			break;
		}

		buf[BUFSIZE - 1] = '\0';

		// printf("client %lu: %s\n", client_info->thread_id, buf);

		if (strncmp(buf, "GET ", 4) == 0){

			if (handle_GET(buf) == false)
				break;

			if (send(client_info->clifd, buf, strlen(buf), 0) == -1){
				perror("send GET");
				break;
			}

		} else if (strncmp(buf, "POST ", 5) == 0){

			if (handle_POST(buf) == false){
				printf("handle_POST error\n");
				break;
			}

		} else {
			sprintf(buf, "HTTP/1.1 400 Bad Request\r\n\r\n");
			send(client_info->clifd, buf, strlen(buf), 0);
		}
	}

	printf("client disconnected\n");

	close(client_info->clifd);
	client_info->used = false;

	pthread_exit(NULL);
}


int server_main(){
	int clifd, len;
	struct sockaddr_in serveraddr, clientaddr;

	client_info_t client_info[MAX_CONNECTIONS];
   
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		error_and_die("socket creation failed...\n");

	const int reuse = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1){
		error_and_die("setsockopt");
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);
   
	if ((bind(listenfd, (SA*)&serveraddr, sizeof(serveraddr))) == -1)
		error_and_die("socket bind failed...\n");
   
	if ((listen(listenfd, 5)) != 0)
		error_and_die("Listen failed...\n");

	printf("server opened on port %d\n", PORT);

	len = sizeof(clientaddr);
   	int c;
	while(1){
		if ((clifd = accept(listenfd, (SA*)&clientaddr, (socklen_t*) &len)) == -1)
			error_and_die("server accept failed...\n");

		for (c = 0; c < MAX_CONNECTIONS; c++){
			if (client_info[c].used == false){
				break;
			}
		}
		if (c == MAX_CONNECTIONS){
			printf("No more connections available\n");
		} else {
			client_info[c].clifd = clifd;
			client_info[c].used = true;

			printf("new client\n");

			pthread_create(&(client_info[c].thread_id), NULL, handle_client, (void*) &(client_info[c]));
		}
	}
}