#pragma once
#include <pthread.h>

#include <string>
#include <queue>

#include "../state/state.h"
#include <httplib/httplib.h>

#define PORT 8080
#define SA struct sockaddr
#define BUFSIZE 16384
#define CLIENT_REFRESH_PERIOD 20 //ms
#define SLEEP_CLIENT 10000

class State;

class Client {
public:
	Client(State* state);
	/* Adds a GET request in the queue */
	void addRequest(std::string);
	/* Thread which sends and receives data to server */
	void* client_thread();
	pthread_mutex_t request_mutex;

private:
	std::queue<std::string> requests;
	State* state;
	httplib::Client* cli;
};

