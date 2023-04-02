#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <queue>

#include "client.h"
#include "../state/state.h"
#include <nlohmann/json.hpp>
#include <httplib/httplib.h>

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

	cli = new httplib::Client("localhost", 8080);
	cli->set_keep_alive(true);
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
		std::cout << "client_thread\n";
		if (started_clock == false){
			started_clock = true;
			time_start = clock();
		}
		else if (((clock() - time_start) * 1000 / CLOCKS_PER_SEC) >= trigger){
			std::cout << "tick" << std::endl;
			started_clock = false;
			json js;
			std::string body;

			auto res = cli->Get("/state");
			if (res->status != 200) {
				std::cerr << httplib::to_string(res.error()) << std::endl;
			} else {
				js = json::parse(res->body);
				state->updateState(js);
			}
		}

		pthread_mutex_lock(&request_mutex);

		if (requests.empty() == false){
			cli->Post("/", requests.front(), "application/json");
			requests.pop();
		}

		pthread_mutex_unlock(&request_mutex);

		usleep(SLEEP_CLIENT);
	}
}