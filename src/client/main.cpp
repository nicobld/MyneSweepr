#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include "state/state.h"
#include "client/client.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "display/display.h"

int sockfd;
char buf[BUFSIZE];
State state;

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
	Display display;

	if(signal(SIGINT, ctrlc) == SIG_ERR){
		ctrlc(0);
	}
	atexit(exitfunc);

	connect_to_server();

	display.update();

	close(sockfd);
	return 0;
}