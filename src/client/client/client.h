#pragma once
#define PORT 8080
#define SA struct sockaddr
#define BUFSIZE 16384

void connect_to_server();

/* Returns pointer to http body */
char* http_get(const char* uri);
void http_post(char* command);