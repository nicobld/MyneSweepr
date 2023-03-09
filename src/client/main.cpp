#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <iostream>
#include <thread>

#include "state/state.h"
#include "client/client.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "display/display.h"

int sockfd;
char buf[BUFSIZE];
State* state;
Client* client;

void ctrlc(int){
	printf("Bye\n");
	close(sockfd);
	exit(EXIT_SUCCESS);
}

void exitfunc(){
	close(sockfd);
	printf("atexit\n");
}

int main(){
	if(signal(SIGINT, ctrlc) == SIG_ERR){
		ctrlc(0);
	}
	atexit(exitfunc);

	state = new State();
	client = new Client(state);
	Display display(state, client);

	// pthread_create(&client_thread, NULL, client->client_thread, NULL);
	std::thread thread(&Client::client_thread, client);

	display.update();

	close(sockfd);
	return 0;
}