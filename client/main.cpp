#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <iostream>
// #include <thread>

#include "state/state.h"
#include "client/client.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "display/display.h"

int sockfd;
char buf[BUFSIZE];
State* state;
Client* client;
Display* display;

void ctrlc(int){
	printf("Bye\n");
	close(sockfd);
	exit(EXIT_SUCCESS);
}

void exitfunc(){
	close(sockfd);
	printf("atexit\n");
}

static void* launch_client(__attribute__((unused)) void* arg){
	client->client_thread();
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	if(signal(SIGINT, ctrlc) == SIG_ERR){
		ctrlc(0);
	}
	atexit(exitfunc);
	bool no_display = false;
	if (argc == 2){
		if (strcmp("no_display", argv[1]) == 0){
			no_display = true;
		} else {
			std::cout << "Usage : \"" << argv[0] << " no_display\" to remove display" << std::endl;
		}
	}

	std::cout << "Hello, world!" << std::endl;

	state = new State();
	client = new Client(state);
	if (!no_display)
		display = new Display(state, client);

	std::cout << "1\n";

	pthread_t threadid;
  
    // pthread_create(&threadid, NULL, &launch_client, NULL);

	// std::thread thread(&Client::client_thread, client);

	std::cout << "2\n";

	if (!no_display)
		display->update();
	else
		while(1) sleep(1);

	close(sockfd);
	return 0;
}