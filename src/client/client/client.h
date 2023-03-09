#pragma once
#include <pthread.h>

#include <string>
#include <queue>

#include "../state/state.h"

#define PORT 8080
#define SA struct sockaddr
#define BUFSIZE 16384
#define CLIENT_REFRESH_PERIOD 50 //ms
#define SLEEP_CLIENT 10000

class State;

class Client {
public:
	Client(State* state);
	/* Adds a GET request in the queue */
	void addRequest(std::string);
	/* Returns pointer to http body */
	std::string http_get(const char* uri);
	/* Thread which sends and receives data to server */
	void* client_thread();
	pthread_mutex_t request_mutex;

private:
	std::queue<std::string> requests;
	State* state;

	void http_post(const char* command);
};

