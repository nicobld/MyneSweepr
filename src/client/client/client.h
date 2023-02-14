#pragma once
#define PORT 8080
#define SA struct sockaddr
#define BUFSIZE 2048

void connect_to_server();

char* http_get(const char* uri);
void http_post(char* command);