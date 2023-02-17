#pragma once

#define BUFSIZE 16384
#define PORT 8080
#define MAX_CONNECTIONS 5
#define SA struct sockaddr

extern int listenfd;

int server_main();